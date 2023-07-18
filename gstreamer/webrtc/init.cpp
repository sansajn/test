/* GStreamer init/deinit test for valgrind.
The sample triggers 62 errors with following

```
...
by 0x4EF7DF7: g_option_context_parse (in /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0.6400.6)
by 0x489F6B6: gst_init_check (in /usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.1603.0)
by 0x489F717: gst_init (in /usr/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0.1603.0)
by 0x10A26E: main (init.cpp:5)
...

ERROR SUMMARY: 62 errors from 62 contexts (suppressed: 0 from 0)
```

complain. */
#include <spdlog/spdlog.h>
#include <gst/gst.h>

int main(int argc, char * argv[]) {
	gst_init(&argc, &argv);
	spdlog::info("GStreamer initialized");

	gst_deinit();
	spdlog::info("Gstreamer deinitialized");

	spdlog::info("done!");
	return 0;
}
