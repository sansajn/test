#include <cstdint>
#include <iostream>

using std::cout;

struct vec3
{
	float x, y, z;
};

void write_buf(void * buf, int offset, vec3 const & v)
{
	float * fbuf = (float *)((uint8_t *)buf+offset);
	*fbuf++ = v.x;
	*fbuf++ = v.y;
	*fbuf = v.z;
}

int main(int argc, char * argv[])
{
	int bufsize = 2*3*sizeof(float);
	uint8_t * buf = new uint8_t[bufsize];

	vec3 u{1.0f, 2.0f, 3.0f};
	vec3 v{10.10f, 20.20f, 33.33f};
	write_buf(buf, 0, u);
	write_buf(buf, 3*sizeof(float), v);

	float * fbuf = (float *)buf;
	for (int i = 0; i < 6; ++i)
		cout << fbuf[i] << ", ";
	cout << "\n";

	delete [] buf;
	return 0;
}
