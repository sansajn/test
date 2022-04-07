#include <thread>
#include <cassert>
#include <libsoup/soup.h>
#include <glib-unix.h>
#include "glib_event_loop.hpp"
#include <iostream>

using std::chrono::milliseconds, std::cout;

namespace {

gboolean quit_loop(glib_event_loop * loop) {
	cout << "ctrl+c (SIGINT) signal catched\n";
	loop->quit();
	return TRUE;
}

}  // namespace


glib_event_loop::glib_event_loop()
	: _ctx{nullptr}
	, _loop{nullptr}
	, _sigint{nullptr} {

	_ctx = g_main_context_new();
	assert(_ctx);
	g_main_context_push_thread_default(_ctx);

	_loop = g_main_loop_new(_ctx, FALSE);
	assert(_loop);

	cout << "glib event loop created\n";
}

glib_event_loop::~glib_event_loop() {
	quit();
	g_main_loop_unref(_loop);
	g_main_context_unref(_ctx);
}

void glib_event_loop::go() {
	cout << "glib event loop running ...\n";
	install_interrupt_handler();
	g_assert(g_main_context_is_owner(_ctx));
	g_main_loop_run(_loop);
}

void glib_event_loop::go_for(milliseconds const & dur) {
	go_while([]{return true;}, dur);
}

bool glib_event_loop::go_while(std::function<bool (void)> cond, std::chrono::milliseconds const & timeout) {
	g_assert(g_main_context_is_owner(_ctx));

	// used for testing purpose it doesn't need to be exact
	milliseconds remains = timeout;

	while (cond() && remains > milliseconds{0}) {
		g_main_context_iteration(_ctx, FALSE);
		std::this_thread::sleep_for(milliseconds{10});
		remains -= milliseconds{10};
	}

	return remains >= milliseconds{0} ? true : false;
}

void glib_event_loop::loop_iteration() {
	g_main_context_iteration(_ctx, FALSE);
}

void glib_event_loop:: quit() {
	g_main_loop_quit(_loop);
}

void glib_event_loop::install_interrupt_handler() {
	assert(!_sigint && "signal already installed");
	_sigint = g_unix_signal_source_new(SIGINT);
	g_source_set_callback(_sigint, (GSourceFunc)quit_loop, this, nullptr);
	g_source_attach(_sigint, _ctx);
}
