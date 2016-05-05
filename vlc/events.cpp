// dumpne vsetky eventy
#include <thread>
#include <string>
#include <iostream>
#include <cassert>
#include <vlc/vlc.h>

using std::cout;
using std::string;

void handle_event(libvlc_event_t const * event, void * user_data);

class vlc_player
{
public:
	vlc_player();
	~vlc_player();
	void open(string const & fname);
	void play();

private:
	libvlc_instance_t * _vlc;
	libvlc_media_t * _media;
	libvlc_media_player_t * _player;
	libvlc_event_manager_t * _media_events;
	libvlc_event_manager_t * _player_events;
};


vlc_player::vlc_player()
	: _media{nullptr}
	, _player{nullptr}
	, _media_events{nullptr}
	, _player_events{nullptr}
{
	_vlc = libvlc_new(0, nullptr);
}

vlc_player::~vlc_player()
{
	libvlc_release(_vlc);
}

void vlc_player::open(string const & fname)
{
	_media = libvlc_media_new_path(_vlc, fname.c_str());
	assert(_media);

	_player = libvlc_media_player_new_from_media(_media);
	assert(_player);

	_media_events = libvlc_media_event_manager(_media);
	assert(_media_events);

	libvlc_event_attach(_media_events, libvlc_MediaMetaChanged, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaSubItemAdded, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaDurationChanged, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaParsedChanged, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaFreed, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaStateChanged, handle_event, nullptr);
	libvlc_event_attach(_media_events, libvlc_MediaSubItemTreeAdded, handle_event, nullptr);

	_player_events = libvlc_media_player_event_manager(_player);
	assert(_player_events);

	libvlc_event_attach(_player_events, libvlc_MediaPlayerMediaChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerNothingSpecial, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerOpening, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerBuffering, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerPlaying, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerPaused, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerStopped, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerForward, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerBackward, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerEndReached, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerEncounteredError, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerTimeChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerPositionChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerSeekableChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerPausableChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerTitleChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerSnapshotTaken, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerLengthChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerVout, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerScrambledChanged, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerCorked, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerUncorked, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerMuted, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerUnmuted, handle_event, nullptr);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerAudioVolume, handle_event, nullptr);
}

void vlc_player::play()
{
	libvlc_media_player_play(_player);
}



int main(int argc, char * argv[])
{
	vlc_player player;
	player.open("ball.mp4");
	player.play();
	std::this_thread::sleep_for(std::chrono::seconds{3});
	return 0;
}

void handle_event(libvlc_event_t const * event, void * user_data)
{
	switch (event->type)
	{
		case libvlc_MediaMetaChanged:
			cout << "MediaMetaChanged" << std::endl;
			break;

		case libvlc_MediaSubItemAdded:
			cout << "MediaSubItemAdded" << std::endl;
			break;

		case libvlc_MediaDurationChanged:
			cout << "MediaDurationChanged" << std::endl;
			break;

		case libvlc_MediaParsedChanged:
			cout << "MediaParsedChanged" << std::endl;
			break;

		case libvlc_MediaFreed:
			cout << "MediaFreed" << std::endl;
			break;

		case libvlc_MediaStateChanged:
			cout << "MediaStateChanged" << std::endl;
			break;

		case libvlc_MediaSubItemTreeAdded:
			cout << "MediaSubItemTreeAdded" << std::endl;
			break;

//		case libvlc_MediaParsedStatus:

		case libvlc_MediaPlayerMediaChanged:
			cout << "MediaPlayerMediaChanged" << std::endl;
			break;

		case libvlc_MediaPlayerNothingSpecial:
			cout << "MediaPlayerNothingSpecial" << std::endl;
			break;

		case libvlc_MediaPlayerOpening:
			cout << "MediaPlayerOpening" << std::endl;
			break;

		case libvlc_MediaPlayerBuffering:
			cout << "MediaPlayerBuffering" << std::endl;
			break;

		case libvlc_MediaPlayerPlaying:
			cout << "MediaPlayerPlaying" << std::endl;
			break;

		case libvlc_MediaPlayerPaused:
			cout << "MediaPlayerPaused" << std::endl;
			break;

		case libvlc_MediaPlayerStopped:
			cout << "MediaPlayerStopped" << std::endl;
			break;

		case libvlc_MediaPlayerForward:
			cout << "MediaPlayerForward" << std::endl;
			break;

		case libvlc_MediaPlayerBackward:
			cout << "MediaPlayerBackward" << std::endl;
			break;

		case libvlc_MediaPlayerEndReached:
			cout << "MediaPlayerEndReached" << std::endl;
			break;

		case libvlc_MediaPlayerEncounteredError:
			cout << "MediaPlayerEncounteredError" << std::endl;
			break;

		case libvlc_MediaPlayerTimeChanged:
			cout << "MediaPlayerTimeChanged" << std::endl;
			break;

		case libvlc_MediaPlayerPositionChanged:
			cout << "MediaPlayerPositionChanged" << std::endl;
			break;

		case libvlc_MediaPlayerSeekableChanged:
			cout << "MediaPlayerSeekableChanged" << std::endl;
			break;

		case libvlc_MediaPlayerPausableChanged:
			cout << "MediaPlayerPausableChange" << std::endl;
			break;

		case libvlc_MediaPlayerTitleChanged:
			cout << "MediaPlayerTitleChanged" << std::endl;
			break;

		case libvlc_MediaPlayerSnapshotTaken:
			cout << "MediaPlayerSnapshotTaken" << std::endl;
			break;

		case libvlc_MediaPlayerLengthChanged:
			cout << "MediaPlayerLengthChanged" << std::endl;
			break;

		case libvlc_MediaPlayerVout:
			cout << "MediaPlayerVout" << std::endl;
			break;

		case libvlc_MediaPlayerScrambledChanged:
			cout << "MediaPlayerScrambledChanged" << std::endl;
			break;

//		case libvlc_MediaPlayerESAdded:
//		case libvlc_MediaPlayerESDeleted:
//		case libvlc_MediaPlayerESSelected:

		case libvlc_MediaPlayerCorked:
			cout << "MediaPlayerCorked" << std::endl;
			break;

		case libvlc_MediaPlayerUncorked:
			cout << "MediaPlayerUncorked" << std::endl;
			break;

		case libvlc_MediaPlayerMuted:
			cout << "MediaPlayerMuted" << std::endl;
			break;

		case libvlc_MediaPlayerUnmuted:
			cout << "MediaPlayerUnmuted" << std::endl;
			break;

		case libvlc_MediaPlayerAudioVolume:
			cout << "MediaPlayerAudioVolum" << std::endl;
			break;

//		case libvlc_MediaPlayerAudioDevice
//		case libvlc_MediaPlayerChapterChanged

		case libvlc_MediaListItemAdded:
			cout << "MediaListItemAdded" << std::endl;
			break;

		case libvlc_MediaListWillAddItem:
			cout << "MediaListWillAddItem" << std::endl;
			break;

		case libvlc_MediaListItemDeleted:
			cout << "MediaListItemDeleted" << std::endl;
			break;

		case libvlc_MediaListWillDeleteItem:
			cout << "MediaListWillDeleteItem" << std::endl;
			break;

//		case libvlc_MediaListEndReached

		case libvlc_MediaListViewItemAdded:
			cout << "MediaListViewItemAdded" << std::endl;
			break;

		case libvlc_MediaListViewWillAddItem:
			cout << "MediaListViewWillAddItem" << std::endl;
			break;

		case libvlc_MediaListViewItemDeleted:
			cout << "MediaListViewItemDeleted" << std::endl;
			break;

		case libvlc_MediaListViewWillDeleteItem:
			cout << "MediaListViewWillDeleteItem" << std::endl;
			break;

		case libvlc_MediaListPlayerPlayed:
			cout << "MediaListPlayerPlayed" << std::endl;
			break;

		case libvlc_MediaListPlayerNextItemSet:
			cout << "MediaListPlayerNextItemSet" << std::endl;
			break;

		case libvlc_MediaListPlayerStopped:
			cout << "MediaListPlayerStopped" << std::endl;
			break;

		case libvlc_MediaDiscovererStarted:
			cout << "MediaDiscovererStarted" << std::endl;
			break;

		case libvlc_MediaDiscovererEnded:
			cout << "MediaDiscovererEnded" << std::endl;
			break;

		case libvlc_VlmMediaAdded:
			cout << "VlmMediaAdded" << std::endl;
			break;

		case libvlc_VlmMediaRemoved:
			cout << "VlmMediaRemoved" << std::endl;
			break;

		case libvlc_VlmMediaChanged:
			cout << "VlmMediaChanged" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStarted:
			cout << "VlmMediaInstanceStarted" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStopped:
			cout << "VlmMediaInstanceStopped" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusInit:
			cout << "VlmMediaInstanceStatusInit" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusOpening:
			cout << "VlmMediaInstanceStatusOpening" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusPlaying:
			cout << "VlmMediaInstanceStatusPlaying" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusPause:
			cout << "VlmMediaInstanceStatusPause" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusEnd:
			cout << "VlmMediaInstanceStatusEnd" << std::endl;
			break;

		case libvlc_VlmMediaInstanceStatusError:
			cout << "VlmMediaInstanceStatusError" << std::endl;
			break;

		default:
			cout << "unknown event" << std::endl;
	}
}

