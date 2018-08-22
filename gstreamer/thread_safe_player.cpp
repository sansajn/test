// thread safe player with event propagation
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <gst/gst.h>
#include <gtk/gtk.h>

using std::string;
using std::mutex;
using std::lock_guard;
using std::cout;
using std::cerr;


class playbin  // TODO: make non copyable
{
public:
	playbin();
	~playbin();
	bool play(std::string const & uri);
	bool stop();
	long position() const;  //!< in ns
	long duration() const;  //!< in ns
	GstElement * to_gst();

private:
	GstElement * _playbin;
};

class player  //!< thread safe player with event propagation
{
public:
	player();
	~player();
	void start();
	void play(string const & uri);
	void stop();
	long position() const;
	long duration() const;
	bool playing() const;
	void join();

	virtual void on_eos();
	virtual void on_position_changed();

private:
	void watch_events();  //!< designed to run in _loop_th thread
	void handle_message(GstMessage * msg);
	void update_playback_info();

	long _position;  //!< in ns
	long _duration;  //!< in ns
	bool _playing;
	mutable std::mutex _pline_state_locker;

	std::thread _loop_th;
	playbin _pbin;
	std::atomic_bool _quit;
};

player::player()
	: _position{0L}
	, _duration{0L}
	, _playing{false}
	, _quit{false}
{}

player::~player()
{
	_quit = true;
	_loop_th.join();
}

void player::start()
{
	_loop_th = std::thread{&player::watch_events, this};
}

void player::play(string const & uri)
{
	_pbin.play(uri);
	lock_guard<mutex> lock{_pline_state_locker};
	_playing = true;
}

void player::stop()
{
	_pbin.stop();
}

long player::position() const
{
	lock_guard<mutex> lock{_pline_state_locker};
	return _position;
}

long player::duration() const
{
	lock_guard<mutex> lock{_pline_state_locker};
	return _duration;
}

bool player::playing() const
{
	lock_guard<mutex> lock{_pline_state_locker};
	return _playing;
}

void player::join()
{
	if (!playing())
		return;

	long diff = duration() - position();
	assert(diff > 0);
	std::this_thread::sleep_for(std::chrono::nanoseconds{diff});

	while (playing())
		;
}

void player::watch_events()
{
	GstBus * bus = gst_element_get_bus(_pbin.to_gst());

	while (!_quit)
	{
		GstMessage * msg = gst_bus_timed_pop_filtered(bus, 100 * GST_MSECOND,
			(GstMessageType)(GST_MESSAGE_ERROR|GST_MESSAGE_EOS|GST_MESSAGE_STATE_CHANGED));

		if (msg)
		{
			handle_message(msg);
			gst_message_unref(msg);
		}
		else  // timer expired
			update_playback_info();
	}

	gst_object_unref(bus);
}

void player::handle_message(GstMessage * msg)
{
	switch (GST_MESSAGE_TYPE(msg))
	{
		case GST_MESSAGE_ERROR:
		{
			GError * err;
			gchar * debug_info;
			gst_message_parse_error(msg, &err, &debug_info);
			cerr << "error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << "\n";
			cerr << "debug information: " << (debug_info ? debug_info : "none") << "\n";
			g_clear_error(&err);
			g_free(debug_info);
			break;
		}

		case GST_MESSAGE_EOS:
		{
			{
				lock_guard<mutex> lock{_pline_state_locker};
				_playing = false;
			}

			on_eos();

			break;
		}

		case GST_MESSAGE_STATE_CHANGED:
		{
			GstState old_state, new_state, pending_state;
			gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);

			if (GST_MESSAGE_SRC(msg) == GST_OBJECT(_pbin.to_gst()))
			{
				cout << "Pipeline state changed from " << gst_element_state_get_name(old_state) << " to " << gst_element_state_get_name(new_state) << "\n";

				lock_guard<mutex> lock{_pline_state_locker};

				if ((old_state == GST_STATE_PLAYING) && (new_state != GST_STATE_PLAYING))
					_playing = false;

				if (new_state == GST_STATE_PLAYING)
				{
					_duration = _pbin.duration();
					assert(_duration > 0);
				}
			}
			break;
		}

		default:
			cerr << "Unexpected message received.\n";
			break;
	}
}

void player::update_playback_info()
{
	if (!_playing)
		return;
	else
	{
		lock_guard<mutex> lock{_pline_state_locker};
		_position = _pbin.position();
		_duration = _pbin.duration();
		assert(_duration > 0);
	}

	on_position_changed();
}


void player::on_eos()
{
	cerr << "End-Of-Stream reached.\n";
}

void player::on_position_changed()
{
	cerr << position() << "/" << duration() << std::endl;
}

playbin::playbin()
{
	_playbin = gst_element_factory_make("playbin", "playbin");
	assert(_playbin && "unable to create a playbin element");
}

playbin::~playbin()
{
	gst_element_set_state(_playbin, GST_STATE_NULL);
	gst_object_unref(_playbin);
	_playbin = nullptr;
}

bool playbin::play(std::string const & uri)
{
	g_object_set(_playbin, "uri", uri.c_str(), nullptr);

	GstStateChangeReturn ret = gst_element_set_state(_playbin, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		cerr << "Unable to set the pipeline to the playing state.\n";
		return false;
	}

	return true;
}

bool playbin::stop()
{
	GstStateChangeReturn ret = gst_element_set_state(_playbin, GST_STATE_READY);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		cerr << "Unable to set the pipeline to the ready state.\n";
		return false;
	}
	return true;
}

long playbin::position() const
{
	gint64 pos = -1;
	if (!gst_element_query_position(_playbin, GST_FORMAT_TIME, &pos))
		cerr << "warning: could not query current position" << std::endl;
	return (long)pos;
}

long playbin::duration() const
{
	gint64 dur = -1;
	if (!gst_element_query_duration(_playbin, GST_FORMAT_TIME, &dur))
		cerr << "warning: could not query duration" << std::endl;
	return (long)dur;
}

GstElement * playbin::to_gst()
{
	return _playbin;
}


int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cerr << "input file missing, exit\n";
		return -1;
	}

	string ifile = argv[1];

	gst_init(&argc, &argv);

	char buf[1024];
	getcwd(buf, sizeof(buf));
	string uri = "file://" + string{buf} + string{"/"} + ifile;

	player p;
	p.start();
	p.play(uri);
	std::this_thread::sleep_for(std::chrono::seconds{5});
	p.stop();
	std::this_thread::sleep_for(std::chrono::seconds{5});
	p.play(uri);
	p.join();

	return 0;
}
