// using thread-default GMainContext sample, see [there](https://developer.gnome.org/programming-guidelines/unstable/main-contexts.html.en#what-is-gmaincontext) for more about contexts in GLib
#include <iostream>
#include <glib.h>

using std::cout, std::endl;

gboolean say_hello(void *) {
	cout << "hello!" << endl;
	return TRUE;
}

int main(int argc, char * argv[]) {
	cout << "press ctrl+c to quit\n";
	
	GMainContext * ctx = g_main_context_new();
	g_main_context_push_thread_default(ctx);
	GMainLoop * loop = g_main_loop_new(ctx, FALSE);

	g_assert(g_main_context_is_owner(ctx));
	
	// we can not use `g_timeout_add_seconds()` there, because it use global-default context, but we loop over thread-default context
	GSource * hello_src = g_timeout_source_new_seconds(1);
	g_source_set_callback(hello_src, (GSourceFunc)say_hello, NULL, NULL);
	g_source_attach(hello_src, ctx);

	g_main_loop_run(loop);  // blocking

	g_main_loop_unref(loop);

	g_source_destroy(hello_src);
	g_source_unref(hello_src);
	
	g_main_context_pop_thread_default(ctx);
	g_main_context_unref(ctx);

	return 0;
}
