#include <iterator>

// iterator
namespace Detail {

template <class ForwardIterator1, class ForwardIterator2, typename T>
void __IterSwap(ForwardIterator1 a, ForwardIterator2 b, T *)
{
	T tmp = *a;
	*a = *b;
	*b = tmp;
};

/*template <typename I>
typename IteratorTraits<I>::ValueType * ValueType(I)
{
	return (typename IteratorTraits<I>::ValueType *)(0);
}*/

template <typename I>
typename std::iterator_traits<I>::value_type * ValueType(I)
{
	return (typename std::iterator_traits<I>::value_type *)(0);
}

};  // Detail

template <class ForwardIterator1, class ForwardIterator2>
void IterSwap(ForwardIterator1 a, ForwardIterator2 b)
{
	Detail::__IterSwap(a, b, Detail::ValueType(a));
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 CopyBackward(BidirectionalIterator1 first, 
	BidirectionalIterator1 last, BidirectionalIterator2 result)
{
	while (first != last)
	{
		--last;
		--result;
		*result = *last;
	}
	return result;
}


namespace Detail {

// sort helpers

template <typename T>
T Median(T const & a, T const & b, T const & c)
{
	if (a < b)
	{
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	}
	else if (a < c)
		return a;
	else if (b < c)
		return c;
	else
		return b;
}

template <class RandomAccessIterator, typename T>
void UnguardedLinearInsert(RandomAccessIterator last, T val)
{
	RandomAccessIterator next = last;
	--next;
	while (val < *next)
	{
		*last = *next;
		last = next;
		--next;
	}
	*last = val;	
}

template <class RandomAccessIterator, typename T>
void LinearInsert(RandomAccessIterator first, RandomAccessIterator last, T *)
{
	T val = *last;
	if (val < *first)
	{
		CopyBackward(first, last, last+1);
		*first = val;
	}
	else
		UnguardedLinearInsert(last, val);
}

template <class RandomAccessIterator>
void InsertionSort(RandomAccessIterator first, RandomAccessIterator last)
{
	if (first == last)
		return;

	for (RandomAccessIterator i = first+1; i != last; ++i)
		LinearInsert(first, i, ValueType(first));
}

template <class RandomAccessIterator, typename T>
RandomAccessIterator UnguardedPartition(RandomAccessIterator first, 
	RandomAccessIterator last, T pivot)
{
	while (true)
	{
		while (*first < pivot)
			++first;
		--last;
		while (*last > pivot)
			--last;
		if (!(first < last))
			return first;
		IterSwap(first, last);
		++first;
	}
}

template <class RandomAccessIterator, typename T>
void __NthElement(RandomAccessIterator first, RandomAccessIterator nth,
	RandomAccessIterator last, T *)
{
	while (last - first > 3)
	{
		RandomAccessIterator cut =
			UnguardedPartition(first, last, T(Median(*first,
				*(first+(last-first)/2), *(last-1))));
		if (cut <= nth)
			first = cut;
		else
			last = cut;
	}
	InsertionSort(first, last);
}

};  // Detail

template <class RandomAccessIterator>
void NthElement(RandomAccessIterator first, RandomAccessIterator nth,
	RandomAccessIterator last)
{
	Detail::__NthElement(first, nth, last, Detail::ValueType(first));
}

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>

int main(int argc, char * argv[])
{
	srand(time(0));

	int data[] = {9, 5, 7, 2, 1};
	std::vector<int> v(data, data+5);
	NthElement(v.begin(), v.begin()+1, v.end());
	std::cout << v[1] << "\n";

	std::vector<int> u;
	for (int i = 0; i < 1000; ++i)
		u.push_back(rand());
	std::vector<int> w(u);

	std::nth_element(u.begin(), u.begin()+31, u.end());

	NthElement(w.begin(), w.begin()+31, w.end());

	std::cout << "std:" << u[31] << " == RcL:" << w[31] << "\n";

	return 0;
}

