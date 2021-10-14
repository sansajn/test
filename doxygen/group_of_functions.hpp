/*! \file 
Free standing function grouping sample shown in Modules tab. */
#include <string_view>

/*! \defgroup liba Library (liba)
This is the first group.
\{ */

using liba_handle = void *;

liba_handle liba_init();
void liba_free(liba_handle h);
int liba_open_channel(liba_handle h);
void liba_close_channel(liba_handle h, int channel);
size_t liba_send(liba_handle h, int channel, std::string_view const text);

/*! \} */


/*! Read from channel.
\ingroup liba */
std::string liba_read(liba_handle h, int channel);
