#pragma once
#include <functional>
#include <chrono>
#include <glib.h>

/*! Creates GLib's main loop with thread-defaut context and ctrl+c handling.
\note the context is set as default to this thread so `event_loop` instance needs to be created at the very beginning of thread main (before any other GLib object using context).
\note Functions like g_timeout_add_seconds (use global-default context) are not working anymore with event_loop, use g_timeout_source_new_seconds instead. */
class event_loop {
public:
	event_loop();
	~event_loop();
	void go();
	void go_for(std::chrono::milliseconds const & dur);
	bool go_while(std::function<bool (void)> cond, std::chrono::milliseconds const & timeout);
	void quit();

	GMainContext * native_context() const {return _ctx;}

private:
	GMainContext * _ctx;
	GMainLoop * _loop;
	GSource * _sigint;
};
