// single thread http sample from http://www.ibm.com/developerworks/aix/library/au-libev/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <evhttp.h>

using std::cout;

void generic_request_handler(evhttp_request * req, void * arg)
{
	evbuffer * returnbuffer = evbuffer_new();
	evbuffer_add_printf(returnbuffer, "Thanks for the request!");
	evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
	evbuffer_free(returnbuffer);
	return;
}

int main(int argc, char * argv[])
{
	short http_port = 5555;
	char const * http_addr = "127.0.0.1";
	
	event_init();
	evhttp * http_server = evhttp_start(http_addr, http_port);
	evhttp_set_gencb(http_server, generic_request_handler, NULL);
	
	cout << "server started on port " << http_port << std::endl;
	
	event_dispatch();
	
	return 0;
}