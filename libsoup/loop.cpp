#include <thread>
#include <cassert>
#include <libsoup/soup.h>
#include <glib-unix.h>
#include "timeout_callback.hpp"
#include "loop.hpp"

using std::chrono::milliseconds;
using std::function;
using namespace std::chrono_literals;

gboolean quit_loop(event_loop * loop) {
	loop->quit();
	return TRUE;
}

event_loop::event_loop()
	: _ctx{nullptr}
	, _loop{nullptr}
	, _sigint{nullptr} {

	_ctx = g_main_context_new();
	assert(_ctx);
	g_main_context_push_thread_default(_ctx);

	_loop = g_main_loop_new(_ctx, FALSE);
	assert(_loop);

	// install interrupt (<ctrl>+c) handler
	_sigint = g_unix_signal_source_new(SIGINT);
	g_source_set_callback(_sigint, (GSourceFunc)quit_loop, this, nullptr);
	g_source_attach(_sigint, _ctx);
}

event_loop::~event_loop() {
	quit();

	g_source_destroy(_sigint);
	g_source_unref(_sigint);

	g_main_loop_unref(_loop);
	g_main_context_pop_thread_default(_ctx);
	g_main_context_unref(_ctx);
}

void event_loop::go() {
	g_main_loop_run(_loop);
}

void event_loop::go_for(milliseconds const & dur) {
	g_assert(g_main_context_is_owner(_ctx));

	timeout_callback quit_cb{dur, (GSourceFunc)quit_loop, this, *this};

	go();  // blocking
}

bool event_loop::go_while(function<bool (void)> cond, milliseconds const & timeout) {
	g_assert(g_main_context_is_owner(_ctx));

	// menat to be used for testing purpose only, it doesn't need to be exact
	milliseconds remains = timeout;
	while (cond() && remains > milliseconds{0}) {
		g_main_context_iteration(_ctx, FALSE);
		std::this_thread::sleep_for(milliseconds{10});
		remains -= milliseconds{10};
	}

	return remains >= 0ms ? true : false;
}

void event_loop::quit() {
	g_main_loop_quit(_loop);
}
