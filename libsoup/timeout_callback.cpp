#include <algorithm>
#include <cassert>
#include "timeout_callback.hpp"

using std::chrono::milliseconds;

timeout_callback::timeout_callback() : _src{nullptr} {}

timeout_callback::timeout_callback(milliseconds interval, GSourceFunc job,
	gpointer data, event_loop const & loop) {

	_src = g_timeout_source_new(interval.count());
	assert(_src);
	g_source_set_callback(_src, job, data, nullptr);
	g_source_attach(_src, loop.native_context());
}

timeout_callback::timeout_callback(timeout_callback && rhs)
	: _src{rhs._src} {
	rhs._src = nullptr;
}

timeout_callback & timeout_callback::operator=(timeout_callback && rhs) {
	std::swap(_src, rhs._src);
	return *this;
}

timeout_callback::~timeout_callback() {
	if (_src) {
		g_source_destroy(_src);
		g_source_unref(_src);
	}
}
