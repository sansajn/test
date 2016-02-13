// multi-thread http server (http://kukuruku.co/hub/cpp/lightweight-http-server-in-less-than-40-lines-on-libevent-and-c-11)
#include <stdexcept>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cstdint>
#include <vector>
#include <evhttp.h>

using std::runtime_error;


int main(int argc, char * argv[])
{
	char const SrvAddress[] = "127.0.0.1";
	uint16_t const SrvPort = 5555;
	int const SrvThreadCount = 4;
	
	try 
	{
		void (*OnRequest)(evhttp_request *, void *) = 
			[](evhttp_request * req, void *) {
				auto * OutBuf = evhttp_request_get_output_buffer(req);
				if (!OutBuf)
					return;   // ze by klient neocakaval odpoved ?
				evbuffer_add_printf(OutBuf, "<html><body><center><h1>Hello World!</h1></center></body></html>");
				evhttp_send_reply(req, HTTP_OK, "", OutBuf);
			};
			
		std::exception_ptr InitExcept;  // TODO: ani neviem ze exception_ptr
		bool volatile IsRun = true;
		evutil_socket_t Socket = -1;
		
		auto ThreadFunc = 
			[&](){
				try 
				{
					std::unique_ptr<event_base, decltype(&event_base_free)> EventBase(event_base_new(), &event_base_free);
					if (!EventBase)
						throw runtime_error("failed to create new base_event.");
					
					std::unique_ptr<evhttp, decltype(&evhttp_free)> EvHttp(evhttp_new(EventBase.get()),&evhttp_free);
					if (!EvHttp)
						throw runtime_error("failed to create new evhttp.");
					
					evhttp_set_gencb(EvHttp.get(), OnRequest, nullptr);
					
					if (Socket == -1)
					{
						auto * BoundSock = evhttp_bind_socket_with_handle(EvHttp.get(), SrvAddress, SrvPort);
						if (!BoundSock)
							throw runtime_error("failed to bind server socket.");
						
						if ((Socket = evhttp_bound_socket_get_fd(BoundSock)) == -1)
							throw runtime_error("failed to get server socket for next instance.");
					}
					else
					{
						if (evhttp_accept_socket(EvHttp.get(), Socket) == -1)
							throw runtime_error("failed to bind server socket for new instance.");
					}
					
					for (; IsRun; )
					{
						event_base_loop(EventBase.get(), EVLOOP_NONBLOCK);
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
				}
				catch (...) 
				{
					InitExcept = std::current_exception();
				}
			};  // ThreadFunc
			
		auto ThreadDeleter = 
			[&](std::thread * t) {
				IsRun = false;
				t->join();
				delete t;
			};
				
		typedef std::unique_ptr<std::thread, decltype(ThreadDeleter)> ThreadPtr;
		typedef std::vector<ThreadPtr> ThreadPool;
			
		ThreadPool Threads;
		for (int i = 0; i < SrvThreadCount; ++i)
		{
			ThreadPtr Thread(new std::thread(ThreadFunc), ThreadDeleter);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			if (InitExcept != std::exception_ptr())
			{
				IsRun = false;
				std::rethrow_exception(InitExcept);  // TODO: use of rethrow_exception
			}
			
			Threads.push_back(std::move(Thread));
		}
			
		std::cout << "Press enter for quit." << std::endl;
		std::cin.get();
		IsRun = false;
	}
	catch (std::exception const & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return 0;
}