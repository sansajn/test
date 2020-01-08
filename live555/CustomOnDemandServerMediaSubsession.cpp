#include <stdexcept>
#include <iostream>
#include <cassert>
#include <GroupsockHelper.hh>
#include "CustomOnDemandServerMediaSubsession.hh"

CustomOnDemandServerMediaSubsession
::CustomOnDemandServerMediaSubsession(UsageEnvironment& env,
				Boolean reuseFirstSource,
				portNumBits initialPortNum,
				Boolean multiplexRTCPWithRTP)
  : ServerMediaSubsession(env),
    fSDPLines(NULL), fReuseFirstSource(reuseFirstSource),
    fLastStreamToken(NULL),
    fAppHandlerTask(NULL), fAppHandlerClientData(NULL) {
  fDestinationsHashTable = HashTable::create(ONE_WORD_HASH_KEYS);
  // Make sure RTP ports are even-numbered:
  fInitialPortNum = (initialPortNum+1)&~1;
  gethostname(fCNAME, sizeof fCNAME);
  fCNAME[sizeof fCNAME-1] = '\0'; // just in case
}

CustomOnDemandServerMediaSubsession::~CustomOnDemandServerMediaSubsession() {
  delete[] fSDPLines;

  // Clean out the destinations hash table:
  while (1) {
    CustomDestinations* destinations
      = (CustomDestinations*)(fDestinationsHashTable->RemoveNext());
    if (destinations == NULL) break;
    delete destinations;
  }
  delete fDestinationsHashTable;
}

char const*
CustomOnDemandServerMediaSubsession::sdpLines() {
  if (fSDPLines == NULL) {
    // We need to construct a set of SDP lines that describe this
    // subsession (as a unicast stream).  To do so, we first create
    // dummy (unused) source and "RTPSink" objects,
    // whose parameters we use for the SDP lines:
    unsigned estBitrate;
    FramedSource* inputSource = createNewStreamSource(0, estBitrate);
    if (inputSource == NULL) return NULL; // file not found

    struct in_addr dummyAddr;
    dummyAddr.s_addr = 0;
    Groupsock* dummyGroupsock = createGroupsock(dummyAddr, 0);
    unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
    RTPSink* dummyRTPSink = createNewRTPSink(dummyGroupsock, rtpPayloadType, inputSource);
    if (dummyRTPSink != NULL && dummyRTPSink->estimatedBitrate() > 0) estBitrate = dummyRTPSink->estimatedBitrate();

    setSDPLinesFromRTPSink(dummyRTPSink, inputSource, estBitrate);
    Medium::close(dummyRTPSink);
    delete dummyGroupsock;
    closeStreamSource(inputSource);
  }

  return fSDPLines;
}

static std::ostream & operator<<(std::ostream & out, in_addr const & addr)
{
	char addr_buf[16];
	inet_ntop(AF_INET, &addr.s_addr, addr_buf, 16);
	addr_buf[15] = '\0';
	out << addr_buf;
	return out;
}

void CustomOnDemandServerMediaSubsession
::getStreamParameters(unsigned clientSessionId,
		      netAddressBits clientAddress,
		      Port const& clientRTPPort,
		      Port const& clientRTCPPort,
		      int tcpSocketNum,
		      unsigned char rtpChannelId,
		      unsigned char rtcpChannelId,
		      netAddressBits& destinationAddress,
		      u_int8_t& /*destinationTTL*/,
		      Boolean& isMulticast,
		      Port& serverRTPPort,
		      Port& serverRTCPPort,
		      void*& streamToken) {
  if (destinationAddress == 0) destinationAddress = clientAddress;
  struct in_addr destinationAddr; destinationAddr.s_addr = destinationAddress;
//  isMulticast = False;
  isMulticast = True;

  if (fLastStreamToken != NULL && fReuseFirstSource) {
    // Special case: Rather than creating a new 'StreamState',
    // we reuse the one that we've already created:
    serverRTPPort = ((CustomStreamState*)fLastStreamToken)->serverRTPPort();
    serverRTCPPort = ((CustomStreamState*)fLastStreamToken)->serverRTCPPort();
    ++((CustomStreamState*)fLastStreamToken)->referenceCount();
    streamToken = fLastStreamToken;
  } else {
    // Normal case: Create a new media source:
    unsigned streamBitrate;
    FramedSource* mediaSource
      = createNewStreamSource(clientSessionId, streamBitrate);

    // Create 'groupsock' and 'sink' objects for the destination,
    // using previously unused server port numbers:
    RTPSink* rtpSink = NULL;
    Groupsock* rtpGroupsock = NULL;

    if (clientRTPPort.num() != 0 || tcpSocketNum >= 0) { // Normal case: Create destinations
      if (clientRTCPPort.num() == 0) {
			throw std::runtime_error{"raw UDP streaming not supported"};
      } else {
	// Normal case: We're streaming RTP (over UDP or TCP).  Create a pair of
	// groupsocks (RTP and RTCP), with adjacent port numbers (RTP port number even).
	// (If we're multiplexing RTCP and RTP over the same port number, it can be odd or even.)
	NoReuse dummy(envir()); // ensures that we skip over ports that are already in use
	for (portNumBits serverPortNum = fInitialPortNum; ; ++serverPortNum) {
	  struct in_addr dummyAddr; dummyAddr.s_addr = 0;

	  serverRTPPort = serverPortNum;
	  rtpGroupsock = createGroupsock(dummyAddr, serverRTPPort);
	  if (rtpGroupsock->socketNum() < 0) {
	    delete rtpGroupsock;
	    continue; // try again
	  }

	  break; // success
	}

	unsigned char rtpPayloadType = 96 + trackNumber()-1; // if dynamic
	rtpSink = createNewRTPSink(rtpGroupsock, rtpPayloadType, mediaSource);
	if (rtpSink != NULL && rtpSink->estimatedBitrate() > 0) streamBitrate = rtpSink->estimatedBitrate();
      }

      // Turn off the destinations for each groupsock.  They'll get set later
      // (unless TCP is used instead):
      if (rtpGroupsock != NULL) rtpGroupsock->removeAllDestinations();

      if (rtpGroupsock != NULL) {
	// Try to use a big send buffer for RTP -  at least 0.1 second of
	// specified bandwidth and at least 50 KB
	unsigned rtpBufSize = streamBitrate * 25 / 2; // 1 kbps * 0.1 s = 12.5 bytes
	if (rtpBufSize < 50 * 1024) rtpBufSize = 50 * 1024;
	increaseSendBufferTo(envir(), rtpGroupsock->socketNum(), rtpBufSize);
      }
    }

    // Set up the state of the stream.  The stream will get started later:
    streamToken = fLastStreamToken
      = new CustomStreamState(*this, serverRTPPort, serverRTCPPort, rtpSink,
			streamBitrate, mediaSource,
			rtpGroupsock);
  }

  // Record these destinations as being for this client session id:
  CustomDestinations* destinations;
  assert(tcpSocketNum < 0 && "only UDP supported");
  destinations = new CustomDestinations(destinationAddr, clientRTPPort, clientRTCPPort);
  fDestinationsHashTable->Add((char const*)clientSessionId, destinations);
}

void CustomOnDemandServerMediaSubsession::startStream(unsigned clientSessionId,
						void* streamToken,
						TaskFunc* rtcpRRHandler,
						void* rtcpRRHandlerClientData,
						unsigned short& rtpSeqNum,
						unsigned& rtpTimestamp,
						ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
						void* serverRequestAlternativeByteHandlerClientData) {
  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  CustomDestinations* destinations
    = (CustomDestinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
  if (streamState != NULL) {
    streamState->startPlaying(destinations, clientSessionId,
			      rtcpRRHandler, rtcpRRHandlerClientData,
			      serverRequestAlternativeByteHandler, serverRequestAlternativeByteHandlerClientData);
    RTPSink* rtpSink = streamState->rtpSink(); // alias
    if (rtpSink != NULL) {
      rtpSeqNum = rtpSink->currentSeqNo();
      rtpTimestamp = rtpSink->presetNextTimestamp();
    }
  }
}

void CustomOnDemandServerMediaSubsession::pauseStream(unsigned /*clientSessionId*/,
						void* streamToken) {
  // Pausing isn't allowed if multiple clients are receiving data from
  // the same source:
  if (fReuseFirstSource) return;

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  if (streamState != NULL) streamState->pause();
}

void CustomOnDemandServerMediaSubsession::seekStream(unsigned /*clientSessionId*/,
					       void* streamToken, double& seekNPT, double streamDuration, u_int64_t& numBytes) {
  numBytes = 0; // by default: unknown

  // Seeking isn't allowed if multiple clients are receiving data from the same source:
  if (fReuseFirstSource) return;

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    seekStreamSource(streamState->mediaSource(), seekNPT, streamDuration, numBytes);

    streamState->startNPT() = (float)seekNPT;
    RTPSink* rtpSink = streamState->rtpSink(); // alias
    if (rtpSink != NULL) rtpSink->resetPresentationTimes();
  }
}

void CustomOnDemandServerMediaSubsession::seekStream(unsigned /*clientSessionId*/,
					       void* streamToken, char*& absStart, char*& absEnd) {
  // Seeking isn't allowed if multiple clients are receiving data from the same source:
  if (fReuseFirstSource) return;

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    seekStreamSource(streamState->mediaSource(), absStart, absEnd);
  }
}

void CustomOnDemandServerMediaSubsession::nullSeekStream(unsigned /*clientSessionId*/, void* streamToken,
						   double streamEndTime, u_int64_t& numBytes) {
  numBytes = 0; // by default: unknown

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    // Because we're not seeking here, get the current NPT, and remember it as the new 'start' NPT:
    streamState->startNPT() = getCurrentNPT(streamToken);

    double duration = streamEndTime - streamState->startNPT();
    if (duration < 0.0) duration = 0.0;
    setStreamSourceDuration(streamState->mediaSource(), duration, numBytes);

    RTPSink* rtpSink = streamState->rtpSink(); // alias
    if (rtpSink != NULL) rtpSink->resetPresentationTimes();
  }
}

void CustomOnDemandServerMediaSubsession::setStreamScale(unsigned /*clientSessionId*/,
						   void* streamToken, float scale) {
  // Changing the scale factor isn't allowed if multiple clients are receiving data
  // from the same source:
  if (fReuseFirstSource) return;

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  if (streamState != NULL && streamState->mediaSource() != NULL) {
    setStreamSourceScale(streamState->mediaSource(), scale);
  }
}

float CustomOnDemandServerMediaSubsession::getCurrentNPT(void* streamToken) {
  do {
    if (streamToken == NULL) break;

    CustomStreamState* streamState = (CustomStreamState*)streamToken;
    RTPSink* rtpSink = streamState->rtpSink();
    if (rtpSink == NULL) break;

    return streamState->startNPT()
      + (rtpSink->mostRecentPresentationTime().tv_sec - rtpSink->initialPresentationTime().tv_sec)
      + (rtpSink->mostRecentPresentationTime().tv_usec - rtpSink->initialPresentationTime().tv_usec)/1000000.0f;
  } while (0);

  return 0.0;
}

FramedSource* CustomOnDemandServerMediaSubsession::getStreamSource(void* streamToken) {
  if (streamToken == NULL) return NULL;

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  return streamState->mediaSource();
}

void CustomOnDemandServerMediaSubsession
::getRTPSinkandRTCP(void* streamToken,
		    RTPSink const*& rtpSink, RTCPInstance const*& rtcp) {
  if (streamToken == NULL) {
    rtpSink = NULL;
    rtcp = NULL;
    return;
  }

  CustomStreamState* streamState = (CustomStreamState*)streamToken;
  rtpSink = streamState->rtpSink();
  rtcp = streamState->rtcpInstance();
}

void CustomOnDemandServerMediaSubsession::deleteStream(unsigned clientSessionId,
						 void*& streamToken) {
  CustomStreamState* streamState = (CustomStreamState*)streamToken;

  // Look up (and remove) the destinations for this client session:
  CustomDestinations* destinations
    = (CustomDestinations*)(fDestinationsHashTable->Lookup((char const*)clientSessionId));
  if (destinations != NULL) {
    fDestinationsHashTable->Remove((char const*)clientSessionId);

    // Stop streaming to these destinations:
    if (streamState != NULL) streamState->endPlaying(destinations, clientSessionId);
  }

  // Delete the "StreamState" structure if it's no longer being used:
  if (streamState != NULL) {
    if (streamState->referenceCount() > 0) --streamState->referenceCount();
    if (streamState->referenceCount() == 0) {
      delete streamState;
      streamToken = NULL;
    }
  }

  // Finally, delete the destinations themselves:
  delete destinations;
}

char const* CustomOnDemandServerMediaSubsession
::getAuxSDPLine(RTPSink* rtpSink, FramedSource* /*inputSource*/) {
  // Default implementation:
  return rtpSink == NULL ? NULL : rtpSink->auxSDPLine();
}

void CustomOnDemandServerMediaSubsession::seekStreamSource(FramedSource* /*inputSource*/,
						     double& /*seekNPT*/, double /*streamDuration*/, u_int64_t& numBytes) {
  // Default implementation: Do nothing
  numBytes = 0;
}

void CustomOnDemandServerMediaSubsession::seekStreamSource(FramedSource* /*inputSource*/,
						     char*& absStart, char*& absEnd) {
  // Default implementation: do nothing (but delete[] and assign "absStart" and "absEnd" to NULL, to show that we don't handle this)
  delete[] absStart; absStart = NULL;
  delete[] absEnd; absEnd = NULL;
}

void CustomOnDemandServerMediaSubsession
::setStreamSourceScale(FramedSource* /*inputSource*/, float /*scale*/) {
  // Default implementation: Do nothing
}

void CustomOnDemandServerMediaSubsession
::setStreamSourceDuration(FramedSource* /*inputSource*/, double /*streamDuration*/, u_int64_t& numBytes) {
  // Default implementation: Do nothing
  numBytes = 0;
}

void CustomOnDemandServerMediaSubsession::closeStreamSource(FramedSource *inputSource) {
  Medium::close(inputSource);
}

Groupsock* CustomOnDemandServerMediaSubsession
::createGroupsock(struct in_addr const& addr, Port port) {
  // Default implementation; may be redefined by subclasses:
  return new Groupsock(envir(), addr, port, 255);
}

RTCPInstance* CustomOnDemandServerMediaSubsession
::createRTCP(Groupsock* RTCPgs, unsigned totSessionBW, /* in kbps */
	     unsigned char const* cname, RTPSink* sink) {
	return nullptr;
}

void CustomOnDemandServerMediaSubsession
::setRTCPAppPacketHandler(RTCPAppHandlerFunc* handler, void* clientData) {
  fAppHandlerTask = handler;
  fAppHandlerClientData = clientData;
}

void CustomOnDemandServerMediaSubsession
::sendRTCPAppPacket(u_int8_t subtype, char const* name,
		    u_int8_t* appDependentData, unsigned appDependentDataSize) {
  CustomStreamState* streamState = (CustomStreamState*)fLastStreamToken;
  if (streamState != NULL) {
    streamState->sendRTCPAppPacket(subtype, name, appDependentData, appDependentDataSize);
  }
}

void CustomOnDemandServerMediaSubsession
::setSDPLinesFromRTPSink(RTPSink* rtpSink, FramedSource* inputSource, unsigned estBitrate) {
  if (rtpSink == NULL) return;

  char const* mediaType = rtpSink->sdpMediaType();
  unsigned char rtpPayloadType = rtpSink->rtpPayloadType();
  AddressString ipAddressStr(fServerAddressForSDP);
  char* rtpmapLine = rtpSink->rtpmapLine();
  char const* rangeLine = rangeSDPLine();
  char const* auxSDPLine = getAuxSDPLine(rtpSink, inputSource);
  if (auxSDPLine == NULL) auxSDPLine = "";

  char const* const sdpFmt =
    "m=%s %u RTP/AVP %d\r\n"
    "c=IN IP4 %s\r\n"
    "b=AS:%u\r\n"
    "%s"
    "%s"
    "%s"
    "a=control:%s\r\n";
  unsigned sdpFmtSize = strlen(sdpFmt)
    + strlen(mediaType) + 5 /* max short len */ + 3 /* max char len */
    + strlen(ipAddressStr.val())
    + 20 /* max int len */
    + strlen(rtpmapLine)
    + strlen(rangeLine)
    + strlen(auxSDPLine)
    + strlen(trackId());
  char* sdpLines = new char[sdpFmtSize];
  sprintf(sdpLines, sdpFmt,
	  mediaType, // m= <media>
	  fPortNumForSDP, // m= <port>
	  rtpPayloadType, // m= <fmt list>
	  ipAddressStr.val(), // c= address
	  estBitrate, // b=AS:<bandwidth>
	  rtpmapLine, // a=rtpmap:... (if present)
	  rangeLine, // a=range:... (if present)
	  auxSDPLine, // optional extra SDP line
	  trackId()); // a=control:<track-id>
  delete[] (char*)rangeLine; delete[] rtpmapLine;

  fSDPLines = strDup(sdpLines);
  delete[] sdpLines;
}


////////// StreamState implementation //////////

static void afterPlayingStreamState(void* clientData) {
  CustomStreamState* streamState = (CustomStreamState*)clientData;
  if (streamState->streamDuration() == 0.0) {
    // When the input stream ends, tear it down.  This will cause a RTCP "BYE"
    // to be sent to each client, teling it that the stream has ended.
    // (Because the stream didn't have a known duration, there was no other
    //  way for clients to know when the stream ended.)
    streamState->reclaim();
  }
  // Otherwise, keep the stream alive, in case a client wants to
  // subsequently re-play the stream starting from somewhere other than the end.
  // (This can be done only on streams that have a known duration.)
}

CustomStreamState::CustomStreamState(CustomOnDemandServerMediaSubsession& master,
                         Port const& serverRTPPort, Port const& serverRTCPPort,
			 RTPSink* rtpSink,
			 unsigned totalBW, FramedSource* mediaSource,
			 Groupsock* rtpGS)
  : fMaster(master), fAreCurrentlyPlaying(False), fReferenceCount(1),
    fServerRTPPort(serverRTPPort), fServerRTCPPort(serverRTCPPort),
    fRTPSink(rtpSink), fStreamDuration(master.duration()),
    fTotalBW(totalBW),
    fMediaSource(mediaSource), fStartNPT(0.0), fRTPgs(rtpGS) {
}

CustomStreamState::~CustomStreamState() {
  reclaim();
}

void CustomStreamState
::startPlaying(CustomDestinations* dests, unsigned clientSessionId,
	       TaskFunc* rtcpRRHandler, void* rtcpRRHandlerClientData,
	       ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
	       void* serverRequestAlternativeByteHandlerClientData) {
  if (dests == NULL) return;

  if (dests->isTCP) {
    // Change RTP and RTCP to use the TCP socket instead of UDP:
    if (fRTPSink != NULL) {
		 throw std::runtime_error{"streaming over TCP not supported"};
    }
  } else {
    // Tell the RTP and RTCP 'groupsocks' about this destination
    // (in case they don't already have it):
    if (fRTPgs != NULL) fRTPgs->addDestination(dests->addr, dests->rtpPort, clientSessionId);
  }

  if (!fAreCurrentlyPlaying && fMediaSource != NULL) {
    if (fRTPSink != NULL) {
      fRTPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);
      fAreCurrentlyPlaying = True;
    }
  }
}

void CustomStreamState::pause() {
  if (fRTPSink != NULL) fRTPSink->stopPlaying();
  fAreCurrentlyPlaying = False;
}

void CustomStreamState::endPlaying(CustomDestinations* dests, unsigned clientSessionId) {
  if (dests->isTCP) {
    if (fRTPSink != NULL) {
      fRTPSink->removeStreamSocket(dests->tcpSocketNum, dests->rtpChannelId);
    }
  } else {
    // Tell the RTP and RTCP 'groupsocks' to stop using these destinations:
    if (fRTPgs != NULL) fRTPgs->removeDestination(clientSessionId);
  }
}

void CustomStreamState::sendRTCPAppPacket(u_int8_t subtype, char const* name,
				    u_int8_t* appDependentData, unsigned appDependentDataSize) {
}

void CustomStreamState::reclaim() {
  // Delete allocated media objects
  Medium::close(fRTPSink); fRTPSink = NULL;

  fMaster.closeStreamSource(fMediaSource); fMediaSource = NULL;
  if (fMaster.fLastStreamToken == this) fMaster.fLastStreamToken = NULL;

  delete fRTPgs;
  fRTPgs = NULL;
}
