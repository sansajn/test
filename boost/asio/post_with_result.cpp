// thread with asio loop
#include <thread>
#include <future>
#include <functional>
#include <chrono>
#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

using std::jthread, std::stop_token;
using std::future, std::promise, std::packaged_task;
using std::bind;
using std::move;
using std::cout, std::endl;
using namespace std::chrono_literals;
namespace asio = boost::asio;

class brain
{
public:
	brain() : _idle{_io} {}

	void go()
	{
		_th = jthread{&brain::loop, this};
	}

	template <typename F>
	void post(F && job)
	{
		asio::post(job);
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

	void name() {cout << "brain" << endl;}
	int meaning_of_life() const {return 42;}

private:
	void loop() {_io.run();}

	jthread _th;
	asio::io_context _io;
	asio::io_context::work _idle;
};

int main(int argc, char * argv[])
{
	brain b;
	b.go();

	b.post([&b]{b.name();});

	auto answer = b.post_with_result([&b]{
		return b.meaning_of_life();
	});

	cout << "the answer is " << answer.get() << "\n";

	std::this_thread::sleep_for(1s);

	cout << "done!\n";
	return 0;
}
