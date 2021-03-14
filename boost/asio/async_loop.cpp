// async_loop with asio_executor sample implementation 
#include <thread>
#include <stop_token>
#include <future>
#include <functional>
#include <vector>
#include <chrono>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

using std::jthread, std::stop_token, std::stop_callback, std::future, std::promise, std::async;
using std::move;
using std::vector;
using std::cout, std::endl;
using namespace std::chrono_literals;
namespace asio = boost::asio;


class asio_executor
{
public:
	asio_executor() : _idle{_io} {}
	void stop() {_io.stop();}
	void run() {_io.run();}

	template <typename F>
	void post(F && f) {
		asio::post(_io, f);
	}

	template<typename F>
	auto post_with_result(F && f) -> future<decltype(f())> {
		using result_type = decltype(f());
		auto p = promise<result_type>{};
		future<result_type> result = p.get_future();

		asio::post(_io, [result = move(p), func = move(f)]() mutable {
			result.set_value(func());
		});

		return result;
	}

private:
	asio::io_context _io;
	asio::io_context::work _idle;
};

template <typename Executor>
class basic_async_loop
{
public:
	template <typename F>
	void post(F && f) {
		_io.post(move(f));
	}

	void async_run() {
		_th = jthread{[this](stop_token st){
			stop_callback stop_cb{st, [this]{
				_io.stop();
			}};

			_io.run();
		}};
	}

	template<typename F>
	auto post_with_result(F && f) -> future<decltype(f())> {return _io.post_with_result(move(f));}

	void request_stop() {_th.request_stop();}
	[[nodiscard]] stop_token get_stop_token() const noexcept {return _th.get_stop_token();}

	void join() {
		if (_th.joinable())
			_th.join();
	}

private:
	jthread _th;
	Executor _io;
};

using async_loop = basic_async_loop<asio_executor>;


class brain
{
public:
	brain() {_loop.async_run();}
	async_loop & loop() {return _loop;}
	bool need_more() {return true;}
	void work_done() {cout << "*"; flush(cout);}

private:
	async_loop _loop;
};

class worker
{
public:
	worker(brain & b) : _b{&b} {}

	void operator()()
	{
		// work hard ..., then notify
		std::this_thread::sleep_for(100ms);
		_b->loop().post([b = this->_b]{b->work_done();});
	}

private:
	brain * _b;
};

class robo_worker
{
public:
	robo_worker(brain & b) : _b{&b} {}

	void operator()(stop_token st)
	{
		future<bool> more;
		do
		{
			// work hard ..., then notify
			std::this_thread::sleep_for(75ms);
			_b->loop().post([b = this->_b]{b->work_done();});

			// need more?
			more = _b->loop().post_with_result([b = this->_b]{return b->need_more();});
		}
		while (!st.stop_requested() && more.get());
	}

private:
	brain * _b;
};



int main(int argc, char * argv[])
{
	brain b;

	vector<future<void>> results;
	for (int i = 0; i < 3; ++i)
		results.push_back(async(worker{b}));

	auto r = async(worker{b});

	worker w1{b}, w2{b};
	w1();
	w2();

	auto u = async(robo_worker{b}, b.loop().get_stop_token());

	for (auto & r : results)
		r.get();

	std::this_thread::sleep_for(500ms);

	b.loop().request_stop();

	u.get();  // wait for robo_worker

	cout << "\ndone!\n";
	return 0;
}
