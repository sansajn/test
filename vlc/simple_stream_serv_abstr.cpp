#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vlc/vlc.h>

using std::string;
using std::to_string;
using std::cout;
using std::ostringstream;

class stream_server
{
public:
	stream_server();
	~stream_server();
	void bind(unsigned port, string const & stream_name);
	void play(string const & file_name);
	void stop();
	void close();
	string rtsp_url() const;
	void join();

private:
	void notify_end_reached();
	void wait_end_reached();
	static void handle_event(libvlc_event_t const * event, void * user_data);

	libvlc_instance_t * _vlc;
	libvlc_media_player_t * _player;
	libvlc_event_manager_t * _player_events;
	unsigned _port;
	string _stream_name;
	std::mutex _vlc_mutex;
	std::condition_variable _end_reached;
	bool _playing;
};

stream_server::stream_server()
	: _vlc{nullptr}, _player{nullptr}, _player_events{nullptr}, _port{0}, _playing{false}
{}

void stream_server::close()
{
	notify_end_reached();

	if (_player_events)
	{
		// TODO: release _player_events
	}

	if (_player)
	{
		libvlc_media_player_stop(_player);
		libvlc_media_player_release(_player);
	}

	if (_vlc)
		libvlc_release(_vlc);
}

stream_server::~stream_server()
{
	close();
}

void stream_server::bind(unsigned port, string const & stream_name)
{
	_port = port;
	_stream_name = stream_name;

	ostringstream sout;
	sout << "#transcode{vcodec=h264,acodec=mpga,ab=128,channels=2,samplerate=44100}"
		<< ":rtp{sdp=rtsp://:" << port << "/" << stream_name << "}";

	string sout_opts = sout.str();

	char const * vlc_argv[] = {
		"--sout", sout_opts.c_str()
	};

	_vlc = libvlc_new(2, vlc_argv);  // load the vlc engine
	assert(_vlc);
}

void stream_server::play(string const & file_name)
{
	if (_player)
		libvlc_media_player_release(_player);

	libvlc_media_t * media = libvlc_media_new_path(_vlc, file_name.c_str());
	assert(media);

	_player = libvlc_media_player_new_from_media(media);
	assert(_player);

	_player_events = libvlc_media_player_event_manager(_player);
	assert(_player_events);
	libvlc_event_attach(_player_events, libvlc_MediaPlayerEndReached, handle_event, this);

	libvlc_media_release(media);

	libvlc_media_player_play(_player);  // start playing

	_playing = true;
}

void stream_server::stop()
{
	if (_player)
		libvlc_media_player_stop(_player);
	_playing = false;
}

string stream_server::rtsp_url() const
{
	return string{"rtsp://localhost:"} + to_string(_port) + "/" + _stream_name;
}

void stream_server::join()
{
	if (_player && (_playing || libvlc_media_player_is_playing(_player)))
		wait_end_reached();
}

void stream_server::handle_event(libvlc_event_t const * event, void * user_data)
{
	stream_server * serv = static_cast<stream_server *>(user_data);

	switch (event->type)
	{
		case libvlc_MediaPlayerEndReached:
		{
			cout << "MediaPlayerEndReached" << std::endl;
			serv->notify_end_reached();
			break;
		}
	}
}

void stream_server::notify_end_reached()
{
	_playing = false;
	std::unique_lock<std::mutex> lock{_vlc_mutex};
	_end_reached.notify_all();
}

void stream_server::wait_end_reached()
{
	std::unique_lock<std::mutex> lock{_vlc_mutex};
	_end_reached.wait(lock);
}

int main(int argc, char * argv[])
{
	string file_name = argc > 1 ? argv[1] : "test.mp4";

	stream_server serv;
	serv.bind(8854, "testStream");
	serv.play(file_name);

	cout << "streaming '" << file_name << "' at " << serv.rtsp_url() << std::endl;

//	std::this_thread::sleep_for(std::chrono::seconds{30});  // wait 30s and then stop streaming
//	serv.stop();

	serv.join();  // wait for server

	cout << "done\n";
	return 0;
}
