#ifndef __RCL_ITERATOR_H__
#define __RCL_ITERATOR_H__

namespace RcL {

// Standard iterator tags 24.4.3
struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};

// Iterator traits 24.4.1
// \todo Nahraď štandardné typi s LowTypes.h
template <class Iterator>
struct IteratorTraits
{
	typedef typename Iterator::DifferenceType DifferenceType;
	typedef typename Iterator::ValueType ValueType;
	typedef typename Iterator::Pointer Pointer;
	typedef typename Iterator::Reference Referene;
	typedef typename Iterator::IteratorCategory IteratorCategory;
};

template <typename T>
struct IteratorTraits<T*>
{
	typedef ptrdiff_t DifferenceType;
	typedef T ValueType;
	typedef T * Pointer;
	typedef T & Reference;
	typedef RandomAccessIteratorTag IteratorCategory;
};

template <typename T>
struct IteratorTraits<T const *>
{
	typedef ptrdiff_t DifferenceType;
	typedef T ValueType;
	typedef T const * Pointer;
	typedef T const & Reference;
	typedef RandomAccessIteratorTag IteratorCategory;
};

namespace Detail {

// some traits helpers

template <typename I>
typename IteratorTraits<I>::ValueType * ValueType(I)
{
	return (typename IteratorTraits<I>::ValueType *)(0);
}

};

// Basic iterator 24.4.2
template <class Category, typename T, typename Distance = ptrdiff_t, 
	typename PointerT = T *, typename ReferenceT = T &>
struct Iterator
{
	typedef T ValueType;
	typedef Distance DifferenceType;
	typedef PointerT Pointer;
	typedef ReferenceT Reference;
	typedef Category IteratorCategory;
};

// Iterator operations 24.4.4
// \note implementuje operácie iba pre random-access-iteratory

template <class RandomAccessIterator>
size_t Distance(RandomAccessIterator first, RandomAccessIterator last)
{
	return size_t(last - first);
}

template <class RandomAccessIterator, typename Distance>
void Advance(RandomAccessIterator & i, Distance n)
{
	i += n;	
}

}  // RcL

#endif

