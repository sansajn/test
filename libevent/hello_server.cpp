// single thread http server (http://kukuruku.co/hub/cpp/lightweight-http-server-in-less-than-40-lines-on-libevent-and-c-11)
#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>

using std::unique_ptr;

int main(int argc, char * argv[])
{
	if (!event_init())
	{
		std::cerr << "Failed to init libevent." << std::endl;
		return -1;
	}
	
	char const SrvAddress[] = "127.0.0.1";  // aka localhost
	uint16_t SrvPort = 5555;
	unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
	if (!Server)
	{
		std::cerr << "failed to init http server." << std::endl;
		return -1;
	}
	
	void (*OnReq)(evhttp_request * req, void *) =  // TODO: fuu ulozenie lambdy
		[](evhttp_request * req, void *) {
			auto * OutBuf = evhttp_request_get_output_buffer(req);
			if (!OutBuf)
				return;  // co znamena prazdny buffer ?
			evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello World!</h1></center></body></html>");
			evhttp_send_reply(req, HTTP_OK, "", OutBuf);
		};
	
	evhttp_set_gencb(Server.get(), OnReq, nullptr);
	if (event_dispatch() == -1)
	{
		std::cerr << "Failed to run messge loop." << std::endl;
		return -1;
	}
	
	return 0;
}
