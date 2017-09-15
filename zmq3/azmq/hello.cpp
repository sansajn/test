// ping-pong sample (inproc sockets?)
#include <atomic>
#include <thread>
#include <array>
#include <string>
#include <memory>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/utility/string_ref.hpp>
#include <azmq/socket.hpp>
#include <azmq/actor.hpp>

namespace asio = boost::asio;
namespace bs = boost::system;
namespace pt = boost::posix_time;
using std::array;
using std::string;
using std::atomic;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::cout;
using std::ostream;
using boost::string_ref;


class server
{
public:
	server(asio::io_service & ios);
	void ping();

	friend ostream & operator<<(ostream & sout, server const & serv)
	{
		return sout << "pings:" << serv._pimpl->_pings << ", pongs:" << serv._pimpl->_pongs;
	}

private:
	struct impl  // TODO: I don't like the name, rename
	{
		atomic<unsigned long> _pongs, _pings;
		array<char, 256> _buf;

		impl() : _pongs{0}, _pings{0}	{}
	};

	using ptr = shared_ptr<impl>;

	static void run(azmq::socket & sock, ptr pimpl);
	static void do_receive(azmq::socket & sock, weak_ptr<impl> pimpl);

	ptr _pimpl;
	azmq::socket _sock;
	array<char, 256> _buf;
};

void schedule_ping(asio::deadline_timer & timer, server & serv);  //!< ping every 250ms


int main(int argc, char * argv[])
{
	asio::io_service ioserv;

	cout << "running ...";
	cout.flush();

	// halt o SIGINT or SIGTERM
	asio::signal_set signals{ioserv, SIGTERM, SIGINT};
	signals.async_wait(
		[&](bs::error_code const &, int){
			ioserv.stop();
		}
	);

	server serv{ioserv};

	asio::deadline_timer timer{ioserv};
	schedule_ping(timer, serv);

	// run for 5 sedonds
	asio::deadline_timer deadline{ioserv, pt::seconds{5}};
	deadline.async_wait(
		[&](bs::error_code const &){
			ioserv.stop();
		}
	);

	ioserv.run();

	cout << "\ndone, result: " << serv << std::endl;

	return 0;
}

server::server(asio::io_service & ios)
	: _pimpl{make_shared<impl>()}
	, _sock{azmq::actor::spawn(ios, run, _pimpl)}
{}

void server::ping()
{
  _sock.send(asio::buffer("PING"));

  _sock.async_receive(asio::buffer(_buf),
	  [this](bs::error_code const & ec, size_t bytes_transferred){
		  if (ec)
			  return;

		  if (string_ref{_buf.data(), bytes_transferred - 1} == "PONG")
			  _pimpl->_pongs += 1;
	  }
  );
}

void server::run(azmq::socket & sock, ptr pimpl)
{
	do_receive(sock, pimpl);
	sock.get_io_service().run();  // blocking
}

void server::do_receive(azmq::socket & sock, weak_ptr<impl> pimpl)
{
	if (ptr p = pimpl.lock())
	{
		sock.async_receive(asio::buffer(p->_buf),
			[&sock, pimpl](bs::error_code const & ec, size_t bytes_transferred){
				if (ec)
					return;

				if (ptr p = pimpl.lock())
				{
					if (string_ref{p->_buf.data(), bytes_transferred-1} != "PING")
						return;  // exit if not PING
					p->_pings += 1;
					sock.send(asio::buffer("PONG"));
					do_receive(sock, pimpl);  // shcedule another receive
				}
			}
		);
	}
}

void schedule_ping(asio::deadline_timer & timer, server & serv)
{
	serv.ping();

	timer.expires_from_now(pt::milliseconds{250});
	timer.async_wait(
		[&](bs::error_code const & ec){
			if (ec)
				return;
			schedule_ping(timer, serv);  // schedule another ping
		}
	);
}
