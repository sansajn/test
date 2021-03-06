// thread with asio loop
#include <thread>
#include <stop_token>
#include <future>
#include <functional>
#include <vector>
#include <chrono>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

using std::jthread, std::stop_token;
using std::future, std::promise, std::async;
using std::bind, std::move;
using std::vector;
using std::cout, std::endl;
using namespace std::chrono_literals;
namespace asio = boost::asio;

class athread
{
public:
	athread() : _idle{_io}
	{}

	void async_run()
	{
		_th = jthread{[this](stop_token && token){
			// TODO: use stop_callback there
			_io.run();
		}};
	}

	void stop();

	template <typename F>
	void post(F && f)
	{
		asio::post(_io, f);
	}

	template<typename F>
	auto post_with_result(F && f) -> future<decltype(f())>
	{
		using result_type = decltype(f());
		auto p = promise<result_type>{};
		future<result_type> result = p.get_future();

		asio::post(_io, [result = move(p), func = move(f)]() mutable {
			result.set_value(func());
		});

		return result;
	}

	// jthread api
	stop_token get_stop_token() const {return _th.get_stop_token();}
	bool request_stop() noexcept {return _th.request_stop();}

private:
	jthread _th;
	asio::io_context _io;
	asio::io_context::work _idle;
};  // athread

class brain
{
public:
	brain() {_loop.async_run();}
	athread & loop() {return _loop;}
	bool need_more() {return true;}
	void work_done() {cout << "*" << endl;}

private:
	athread _loop;
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


// TODO: implement brain_handle safe brain handle

class robo_worker
{
public:
	robo_worker(brain & b) : _b{&b} {}

	void operator()(stop_token && token)
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
		while (!token.stop_requested() && more.get());
	}

private:
	brain * _b;
};

int main(int argc, char * argv[])
{
	brain b;

//	a.loop().post([&a]{a.say_name();});

//	auto answer = a.loop().post_with_result([&a]{
//		return a.meaning_of_life();
//	});

//	cout << "the answer is " << answer.get() << "\n";

	vector<future<void>> results;
	for (int i = 0; i < 3; ++i)
		results.push_back(async(worker{b}));

	auto r = async(worker{b});

//	worker w1{b}, w2{b};
//	w1();
//	w2();

	auto u = async(robo_worker{b}, b.loop().get_stop_token());

	for (auto & r : results)
		r.get();

	std::this_thread::sleep_for(500ms);

	b.loop().request_stop();

	u.get();  // wait for robo_worker

	cout << "done!\n";
	return 0;
}
