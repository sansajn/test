# Implementing custom iterator

Guide to implement *random access* iterator working with parallel `std::transform` algorithm to support parallelization for pixel algorithms (such as raytracer) ...

- The story behind
- Implementing input iterator
- Implementing forward iterator


> TODO: link implementation files `input_it/forward_it ...` files

# The story behind

I've recently go through great raytracing related book where "raytracing" loop was implemented more or less this way

```c++
RGBColor	L;

Ray ray;
ray.o = eye;

int n = sqrt(num_samples);

for (int r = 0; r < vres; r++) {			// up
	for (int c = 0; c < hres; c++) {		// across
		L = black;
		
		for (int p = 0; p < n; p++)		// up pixel
			for (int q = 0; q < n; q++) {	// across pixel
				Point2D sp = sampler->sample_unit_square();
				pp.x = c - 0.5 * hres + sp.x;
				pp.y = r - 0.5 * vres + sp.y;
				ray.d = get_direction(pp);
				L += tracer->trace_ray(ray, depth);
			}
										
		L /= num_samples;
		display_pixel(r, c, L);
	}
}
```

It is simple, we calculate light for each pixel in output image by casting ray in "right" direction.

The book was written in year 2006 so the raytracer is written in *C++98* without any parallelization support. Because calculating light for each pixel is independend to other pixels around raytracing loop is ideal candidate for parallelization. 

In *C++17* we can implement parallel raytracing with replacing our for based loop with parallel `std::transform` algorithm. We "only" need iterator allow as to iterate through each pixel and function calculate light for that pixel. Our new raytracing loop implementation now become

```c++
auto pixel_rng = pixel_pos_view(vp.hres, vp.vres);
transform(std::execution::par, begin(pixel_rng), end(pixel_rng), begin(pixels),
	[&](pair<size_t, size_t> pixel_pos){  // (c,r)
		RGBColor L{black};
		Ray ray;
		ray.o = eye;

		auto & [c, r] = pixel_pos;
		for (int p = 0; p < n; ++p) {  // up pixel
			for (int q = 0; q < n; ++q) {  // across pixel
				Point2D sp = sampler->sample_unit_square();
				Point2D pp;  // sample point on a pixel
				pp.x = c - 0.5 * hres + sp.x;
				pp.y = r - 0.5 * vres + sp.y;
				ray.d = get_direction(pp);
				L += tracer->trace_ray(ray, 0);
			}
		}

		L /= num_samples;

		return map_to_pixel_color(L);
	});
```

> **note**: some weeks after the implementation I've saw Matt Godbolt in his [*Path Tracing Three Ways: A Study of C++ Style*](https://www.youtube.com/watch?app=desktop&v=HG6c4Kwbv4I) CppCon talk where he described `cartesian_product` view serving for similar purpose ...

where dereferencing our new pixel position iterator returns `pair<size_t, size_t>` representing pixel position as *(column, row)* pair. The lambda function takes pixel position pair and calculate light for pixel on that position.

This post series will describe how to implement pixel position iterator capable parallelizing pixel algorithms like raytracer described in the book.

The first post is this *the story behind* post, then I will describe *input*, *forward*, *bidirectional* and *random access* iterator implementation each in separate post.


# Implementing input iterator

Parallel `std::transform` algorithm requires at least *forward* iterator to work with, but before we can implement *forward* iterator wee need to start with *input* iterator implementation.

I will use *The C++ Standard Library: A Tutorial and Reference* book as a reference for implementing *input* iterator. The book says that *input* iterator should support following operations

> TODO: improve book reference

```
*iter;  // read access to the actual element
iter->member;  // read accesss to a member of the actual element
++iter;  // steps forward (returns new position)
iter++;  // steps forward
iter1 == iter2;  // returns whether two iterators are equal
iter1 != iter2;  // returns whether two iterators are not equal
TYPE(iter);  // copy constructor
```

In *C++* these requiremens can be expressed this way

```c++
template <typename It>
bool input_iterator_api_available() {
	It it1, it2;
	*it1;  // access value
	it1->first;  // access member variable
	++it1;  // pre increment
	it1++;  // post increment
	it1 == it2;  // equal operator
	it1 != it2;  // not equal operator
	It it3{it1};  // copy constructor
	return true;
}
```

We can then write *Catch2* test for our iterator implementation this way

```c++
TEST_CASE("input iterator should allow following expressions", 
	"[input][iterator]") {
	REQUIRE(input_iterator_api_available<pixel_pos_view>());
}
```

where `pixel_pos_view` class our *input* iterator implementation and besides of iterator API it implements `begin()` and `end()` member functions returning *begin* and *end* iterators. So we can write

```c++
auto pixel_rng = pixel_pos_view{3,3};
auto it_beg = begin(pixel_rng),
	it_end = end(pixel_rng);
```

to get *begin* and *end* pixel position iterators. We can also use our iterator with`std::transform` (the linear one) algorithm

```c++
vector<uint8_t> pixels(3*3);  // 9 pixel buffer
auto pixel_rng = pixel_pos_view{3,3};
transform(begin(pixels_rng), end(pixel_rng), begin(pixels), 
	[](pair<size_t, size_t> pixel_pos){
		return random_value();
	});
```

to set `pixels` output buffer/image pixels each to random value.

See `input_it.cpp` for the full `pixel_pos_view` implementation, the only interesting part there is *pre-increment* operator implementation

```c++
pixel_pos_view & operator++() {
	auto & [c, r] = _pos;
	c += 1;
	if (c >= _w) {
		c = 0;

		r += 1;
		if (r >= _h)
			r = _h;  // end of range (0, _h)
	}
	return *this;
}
```

and comparison operator implementation 

```c++
//! comparison for for-each loop (works only for it == end(r) where r is pixel_pos_view instance)
bool operator==(pixel_pos_view const & rhs) const {
	return _h == _pos.second && (rhs._w == 0 && rhs._h == 0);
}
```

which needs return `true` only in case we compare against *end* iterator (e.g. `it == end(pixel_rng)`), otherwise we can return `false`.


In `test_it.hpp` file there is `input_iterator_api_implemented<>` template structure implementation which allows testing input iterator implementation e.g. for *pre-increment* this way

```c++
TEST_CASE("following should work for input itetrator",
	"[input][iterator]") {
	input_iterator_api_implemented<pixel_pos_view> iter;
	
	SECTION("pre increment") {
		REQUIRE(iter.pre_increment());
	}
}
```

We implemented short program to generate grayscale gradient image this way

```c++
constexpr size_t w = 400,
	h = 300;

uint8_t pixels[w*h] = {0};  // grayscale pixels 
pixel_pos_view pos{w, h};
transform(begin(pos), end(pos), begin(pixels),
	[w, h](pair<size_t, size_t> const & pos){  // (column, row) position
		double x = pos.first / double(w),
			y = pos.second / double(h),
			distance = sqrt(x*x + y*y);
		return static_cast<uint8_t>(ceil(255.0 * distance/sqrt(2.0)));
	});

save_grayscale(pixels, w, h, gradient_image);
```

which produce following PNG image

![gradient image](gradient.png)

see *we can use transform with input iterator* test case in `input_it.cpp` file.


# Implementing forward iterator

In addition to *input* iterator *forward* iterator adds *default constructor*, *assign operator*, *post-increment* operator must return old value and *comparison* operator must work in all cases (not only against *end* iterator).

The *The C++ Standard Library: A Tutorial and Reference* (Table 9.4) book says that *forward* iterator should support following operations

```
*iter;  // read access to the actual element
iter->member;  // read accesss to a member of the actual element
++iter;  // steps forward (returns new position)
iter++;  // steps forward (returns old position)
iter1 == iter2;  // returns whether two iterators are equal
iter1 != iter2;  // returns whether two iterators are not equal
TYPE();  // default conctructor
TYPE(iter);  // copy constructor
iter1 = iter2;  // assigns an iterator
```

In *C++* these requiremens can be expressed this way

```c++
template <typename Iter>
bool forward_iterator_api_available() {
	input_iterator_api_available<Iter>();
	Iter it1, it2;
	it1 = it2;  // assign operator
	return true;
}
```

implemented again in `test_it.hpp` file. We can then write [Catch2][Catch2] test for our iterator implementation this way

```c++
TEST_CASE("forward iterator should allow following expressions",
	"[forward][iterator]") {
	REQUIRE(input_iterator_api_available<pixel_pos_view>());
	REQUIRE(forward_iterator_api_available<pixel_pos_view>());
}
```

// TODO: we already do not explicitly call `input_iterator_api_available` in code

where we reused `input_iterator_api_available<>()` template function from previous post to verify *input* iterator behaviour (*forward* iterator should also work as *input* iterator).

> TODO: missing unit test implementation description

In case of template inheritance we need refer to base member variables `pos1` as `this->pos1` or `base::pos1` otherwise compiler become angry to us.

I've tryed to implement `forward_iterator_api_implemented::assign()` test this way

```c++
bool assign() {
	Iter pos2;
	REQUIRE(pos1 != pos2);
	pos2 = pos1;
	REQUIRE(pos1 == pos2);
	++pos1;
	REQUIRE(pos1 != pos2);
	++pos2;
	REQUIRE(pos1 == pos2);
	return true;
}
```

but that ends with *error: 'pos1' was not declared in this scope; did you mean 'pos2'?* compiler error. Even after 18 years *C++* can suprise me from time to time. Thanks to the *stackoverflow* I've found the behaviour description in [Why am I getting errors when my template-derived-class uses a member it inherits from its template-base-class?](https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members).

*Forward* iterator implementation is available in `forwad_it.cpp` file. As in case of *input* iterator there is also program/test (and its parallel version) to generate grayscale gradient image which produce following PNG image

![gradient image](gradient.png)

see *we can use parallel transform with forward iterator* test case.


> TODO: explain why we implemented random access iterator (parallel transform require only forward iterator)?


# Implementing bidirectional iterator

In addition to *forward* iterator *bidirectional* iterator adds *pre/post decrement* operators.

The *The C++ Standard Library: A Tutorial and Reference* (Table 9.5) book says that *bidirectional* iterator should support following operations in addtion of *forward* iterator

```
// all forward iterator operations
--iter;  // steps backward (returns new position)
iter--;  // steps backward (returns old position)
```

In *C++* these requiremens can be expressed this way

```c++
template <typename Iter>
bool bidirectional_iterator_api_available() {
	forward_iterator_api_available<Iter>();
	Iter pos4{2,2};
	++pos4;
	++pos4;
	--pos4;  // pre decrement
	pos4--;  // post decrement
	return true;
}
```

implemented again in `test_it.hpp` file. We can then write [Catch2][Catch2] test for our iterator implementation this way

```c++
TEST_CASE("bidirectional iterator should allow following expressions",
	"[bidirectional][iterator]") {
	REQUIRE(bidirectional_iterator_api_available<pixel_pos_view>());
}
```

where `bidirectional_iterator_api_available<>()` implementation reuse *forward* iterator requrements implementation from `forward_iterator_api_available<>()`.

In case of verifying iterator implementation only tests for *post/pre decrement* needs to be implemented, see `pre_decrement()` and `post_decrement()` member function implementation for `bidirectional_iterator_api_iplemented<>` template in `test_it.hpp` file (the code is not interesting enough to paste it there). The rest of unit test code is reused from `forward_iterator_api_implemented<>` template implementation.

*Bidirectional* iterator implementation is available in `bidirectional_it.cpp` file. As in case of *input* and *forward* iterators there is also program/test (and its parallel version) to generate grayscale gradient image which produce following PNG image

![gradient image](gradient.png)

see *we can use parallel transform with bidirectional iterator* test case.


# Implementing random access iterator

Compared to *bidi* RA introuce whole bunch of new operators *random access*, *plus/minus assign*, *plus/minus*, *less/greater* and *less/greater or equal* operators.

The *The C++ Standard Library: A Tutorial and Reference* (Table 9.6) book says that *random access* iterator should support following operations


```
// all bidirectional iterator operations
iter[n];  // access to the element that has index n 
iter+=n;  // steps n elements forward (or backward, if n is negative)
iter-=n;  // steps n elements backward (or forward, if n is negative)
iter+n;  // returns the iterator of the nth element
n+iter;
iter-n;
n-iter;
iter1-iter2;
iter1<iter2;
iter1>iter2;
iter1<=iter2;
iter1>=iter2;
```

In *C++* these requiremens can be expressed this way

```c++
template <typename Iter>
bool random_access_iterator_api_available() {
	bidirectional_iterator_api_available<Iter>();
	Iter it5{5,5};
	it5[3];  // n-th element access
	it5 += 3;  // step n elements forward
	it5 - 3;  // nth previous element
	it5 -= 3;  // step n elements backward
	it5 + 3;  // nth next element
	3 + it5;
	Iter it6 = it5+3;
	it6 - it5;  // distance
	it5 < it6;  // pos5 before pos6
	it5 > it6;  // pos5 after pos6
	it5 <= it6;  // pos5 not after pos6
	it5 >= it6;  // pos5 not before pos6
	return true;
}
```

implemented again in `test_it.hpp` file. We can then write [Catch2][Catch2] test for our iterator implementation this way

```c++
TEST_CASE("random access iterator should allow following expressions",
	"[random][iterator]") {
	REQUIRE(random_access_iterator_api_available<pixel_pos_view>());
}
```

where `random_access_iterator_api_available<>()` implementation reuse *forward* iterator requrements implementation from `bidirectional_iterator_api_available<>()`.





# TODOs

- [x] write guide for input iterator
- [x] write guide for forward iterator
- [ ] write guide for bidirectional iterator
- [ ] write guide for random access iterator
- [ ] reusable unit tests for input, forward, bidirectional and random iterator implementation
- [ ] solve *Using copy-ctor but class X has a trivial copy-ctor but non trivial assign operator* clang warning
- [x] integrate to raytrace engine


[Catch2]: https://github.com/catchorg/Catch2
