#pragma once
#include <chrono>
#include <boost/noncopyable.hpp>
#include <glib.h>
#include "loop.hpp"

/*! Timeout callback wrapper for thread-defaut context loops.
\code
gboolean job(gpointer data) {cout << "hello!\n";}

void foo() {
	event_loop loop;
	timeout_callback cb{1s, job, nullptr, loop};
	loop.go();
}
\endcode
\sa event_loop */
class timeout_callback : private boost::noncopyable {
public:
	timeout_callback();

	timeout_callback(std::chrono::milliseconds interval, GSourceFunc job, gpointer data,
		event_loop const & loop);

	timeout_callback(timeout_callback && rhs);
	timeout_callback & operator=(timeout_callback && rhs);
	~timeout_callback();

private:
	GSource * _src;
};
