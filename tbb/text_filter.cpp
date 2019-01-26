#include <iostream>
#include <tbb/pipeline.h>
#include <tbb/tick_count.h>
#include <tbb/task_scheduler_init.h>

using std::cout;
using namespace tbb;

class my_buffer
{
public:
	char * begin() {return _storage+1;}
	char const * begin() const {return _storage+1;}
	char * end() {return _end;}
	char const * end() const {return _end;}
	void set_end(char * p) {_end = p;}
	size_t max_size() const {return buffer_size;}
	size_t size() const {return _end - begin();}
	
private:
	static size_t const buffer_size = 10000;
	char * _end;
	char _storage[buffer_size+1];  //<! _storage[0] holds the last character of the previous buffer
};


static char const * INPUT_FILE_NAME = "input.txt";
static char const * OUTPUT_FILE_NAME = "output.txt";


class input_filter : public filter
{
public:
	static size_t const n_buffer = 8;
	
	input_filter(FILE * input_file)
		: filter{serial_in_order}
		, _input_file{input_file}
		, _next_buffer{0}
		, _last_char_of_previous_buffer{' '}
	{}
	
private:
	void * operator()(void *)
	{
		my_buffer & b = _buffer[_next_buffer];
		_next_buffer = (_next_buffer + 1) % n_buffer;
		size_t n = fread(b.begin(), 1, b.max_size(), _input_file);
		if (!n)
			return nullptr;  // end of file
		else
		{
			b.begin()[-1] = _last_char_of_previous_buffer;
			_last_char_of_previous_buffer = b.begin()[n-1];
			b.set_end(b.begin()+n);
			return &b;
		}
	}
	
	FILE * _input_file;
	size_t _next_buffer;
	char _last_char_of_previous_buffer;
	my_buffer _buffer[n_buffer];
};

class transform_filter : public filter
{
public:
	transform_filter() : filter{parallel} {}

	void * operator()(void * item)
	{
		my_buffer & b = *static_cast<my_buffer *>(item);
		int prev_char_is_space = b.begin()[-1] == ' ';
		for (char * s = b.begin(); s != b.end(); ++s)
		{
			if (prev_char_is_space && islower((unsigned char)*s))
				*s = toupper(*s);
			prev_char_is_space = isspace((unsigned char)*s);
		}
		return &b;
	}
};

class output_filter : public filter
{
public:
	output_filter(FILE * output_file)
		: filter{serial_in_order}
		, _output_file{output_file}
	{}

	void * operator()(void * item)
	{
		my_buffer & b = *static_cast<my_buffer *>(item);
		int n = (int)fwrite(b.begin(), 1, b.size(), _output_file);
		if (n <= 0)
		{
			fprintf(stderr, "Can't write into %s file\n", OUTPUT_FILE_NAME);
			exit(1);
		}
		return nullptr;
	}

private:
	FILE * _output_file;
};

int run_pipeline(int nthreads)
{
	FILE * input_file = fopen(INPUT_FILE_NAME, "r");
	if (!input_file)
	{
		perror(INPUT_FILE_NAME);
		return 0;
	}

	FILE * output_file = fopen(OUTPUT_FILE_NAME, "w");
	if (!output_file)
	{
		perror(OUTPUT_FILE_NAME);
		return 0;
	}

	pipeline pipe;

	input_filter input{input_file};
	pipe.add_filter(input);

	transform_filter transform;
	pipe.add_filter(transform);

	output_filter output{output_file};
	pipe.add_filter(output);

	// run pipeline
	tick_count t0 = tick_count::now();
	pipe.run(input_filter::n_buffer);
	tick_count t1 = tick_count::now();

	pipe.clear();

	fclose(output_file);
	fclose(input_file);

	if (nthreads == 1)
		printf("single thread run time = %g\n", (t1-t0).seconds());
	else
		printf("parallel run time = %g\n", (t1-t0).seconds());

	return 1;
}

int main(int argc, char * argv[])
{
	{  // single threaded run
		task_scheduler_init init{1};
		if (!run_pipeline(1))
			return 1;
	}

	{  // parallel run
		task_scheduler_init init;
		if (!run_pipeline(0))
			return 1;
	}
	
	return 0;
}
