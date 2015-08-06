// implementácia memory-stream
#include <iostream>
#include <cstdint>

using std::cout;


/*! Umožnuje zápis heterogénnych dát do bloku pamäte. */
class memory_stream
{
public:
	memory_stream(size_t size) 
		: _buf(new uint8_t[size]), _cur(_buf), _size(size) 
	{}

	~memory_stream() {close();}

	template <typename T>
	memory_stream & operator<<(T const & val)
	{
		T * tbuf = (T *)_cur;
		*tbuf++ = val;
		_cur = (void *)tbuf;
		return *this;
	}

	template <typename R = void>
	R * buf(size_t offset = 0) const {return (R *)((uint8_t *)_buf + offset);}

	size_t size() {return _size;}

	void close()
	{
		delete [] _buf;
		_buf = nullptr;
		_cur = nullptr;
		_size = 0;
	}

private:
	uint8_t * _buf;
	void * _cur;  // current buffer position
	size_t _size;
};


// custom adaptacia

struct vec3
{
	float x, y, z;
	vec3 & operator=(vec3 const & rhs) = delete;
};

memory_stream & operator<<(memory_stream & out, vec3 const & v)
{
	cout << "operator<<(..., vec3)\n";
	out << v.x << v.y << v.z;
	return out;
}


void test_native()
{
	memory_stream mout(10*sizeof(int)+3*sizeof(double));
	for (int i = 0; i < 10; ++i)
		mout << i*i;

	for (int i = 0; i < 3; ++i)
		mout << (i+1)*1.11;

	int * buf = (int *)mout.buf();
	for (int i = 0; i < 10; ++i)
		cout << buf[i] << " ";
	cout << std::endl;

	double * dbuf = mout.buf<double>(10*4);
	for (int i = 0; i < 3; ++i)
		cout << dbuf[i] << " ";
	cout << std::endl;
}

void test_adaptation()
{
	memory_stream mout(3*sizeof(vec3));
	
	mout << vec3{1.11, 2.22, 3.33};
	mout << vec3{0.11, 0.22, 0.33};
	mout << vec3{11.1, 22.1, 33.3};

	float * buf = mout.buf<float>();
	for (int i = 0; i < 9; ++i)
		cout << buf[i] << " ";
	cout << std::endl;
}

int main(int argc, char * argv[])
{
	test_native();
	test_adaptation();
	return 0;
}

