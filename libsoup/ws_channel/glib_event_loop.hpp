#pragma once
#include <chrono>
#include <functional>
#include <glib.h>

/*! Creates GLib's main loop with unique context.
\note the context is set as default to this thread so `event_loop` instance needs to be created at the very beginning of thread main (before any other GLib object using context). */
class glib_event_loop {
public:
	glib_event_loop();
	~glib_event_loop();
	void go();  //!< \note blocking
	void go_for(std::chrono::milliseconds const & dur);
	bool go_while(std::function<bool (void)> cond, std::chrono::milliseconds const & timeout);
	void loop_iteration();
	void quit();

private:
	/*! Install `<ctr+c>` (SIGINT) interrupt handler. */
	void install_interrupt_handler();

	GMainContext * _ctx;
	GMainLoop * _loop;
	GSource * _sigint;
};
