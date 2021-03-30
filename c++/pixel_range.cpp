// iterate over image pixels address with range sample
#include <algorithm>
#include <utility>
#include <iostream>
using std::pair, std::cout, std::transform, std::begin, std::end;

class pixel_range {
public:
	pixel_range(size_t w, size_t h) 
		: _w{w}, _h{h}
	{}

	void operator++() {
		++_c;
		if (_c >= _w) {
			_c = 0;

			++_r;
			if (_r >= _h)
				_r = _h;  // end of range
		}
	}

	pair<size_t, size_t> operator*() const {
		return {_c, _r};
	}

	pixel_range begin() {return *this;}
	pixel_range end() {return pixel_range{0, 0};}

	// comparsion for for-each loop
	bool operator==(pixel_range const & rhs) const {
		return _h == _r && rhs._w == 0 && rhs._h == 0;
	}

private:
	size_t _w, _h, _c = 0, _r = 0;
};

int main(int argc, char * argv[]) {
	auto pixels = pixel_range{3, 3};
	for (auto p : pixels)
		cout << p.first << "," << p.second << "\n";

	// test with transform
	char image[9];
	auto pixels2 = pixel_range{3, 3};
	transform(begin(pixels2), end(pixels2), begin(image), [](pair<size_t, size_t> p){
		return p.first + p.second;
	});

	cout << "\nimage:\n";
	for (int r = 0; r < 3; ++r) {
		for (int c = 0; c < 3; ++c) {
			cout << (int)image[c+(r*3)] << " ";
		}
		cout << "\n";
	}
		
	cout <<"done!\n";
	return 0;
}