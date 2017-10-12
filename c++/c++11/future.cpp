// zatial neskompiloovatelne

#include <future>
#include <vector>
#include <iostream>

class aio
{
	class io_request
	{
	private:
		std::streambuf * is;
		unsigned read_count;
		std::promise<std::vector<char>> p;

	public:
		explicit io_request(std::streambuf & is_, unsigned count_)
			: is(&is_), read_count(count_)
		{}

		io_request(io_request && rhs)
			: is(rhs.is), read_count(rhs.read_count), p(std::move(rhs.p))
		{}

		io_request():
			is(0), read_count(0)
		{}

		std::future<std::vector<char>> get_future()
		{
			return p.get_future();
		}

		void process()
		{
			try 
			{
				/* \note do is mozu data chodit postupne, preto ten
					amount_count */
				std::vector<char> buffer(read_count);

				unsigned amount_read = 0;
				while ((amount_read != read_count) &&
					(is->sgetc() != std::char_traits<char>::eof()))
				{
					amount_read += is->sgetn(&buffer[amount_read],
						read_count - amount_read)
				}

				buffer.resize(amount_read);

				p.set_value(std::move(buffer));
			}
			catch (...)
				p.set_exception(std::current_exception());
		}

	};  // io_request

	thread_safe_queue<io_request> request_queue;
	std::atomic_bool done;

	void io_thread()
	{
		while (!done)
		{
			io_request req = request_queue.pop();
			req.process();
		}
	}


	std::thread iot;
	
public:
	aio()
		: done(false), iot(&aio::io_thread, this)
	{}

	std::future<std::vector<char>> queue_read(std::streambuf & is,
		unsigned count)
	{
		io_request req(is, count);
		std::future<std::vector<char>> f(req.get_future());
		request_queue.push(std::move(req));
		return f;
	}

	~aio()
	{
		done = true;
		request_queue.push(io_request());
		iot.join();
	}
};

void do_stuff()
{}

void process_data(std::vector<char> v)
{
	for (auto & e : v)
		std::cout << e;
	std::cout << std::endl;
}

int main()
{
	aio async_io;

	std::filebuf f;
	f.open("my_file.data", std::ios::in|std::ios::binary);
	
	std::future<std::vector<char>> fv = async_io.queue_read(f, 1048576);

	do_stuff();

	process_data(fv.get());

	return 0;
}

