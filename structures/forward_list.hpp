#pragma once
#include <iterator>
#include <cstddef>
#include <cassert>

template <typename T>
struct fwd_list_node
{
	T value;
	fwd_list_node * next;
};

template <typename T>
struct fwd_list_iterator
{
	using self = fwd_list_iterator<T>;
	using value_type = T;
	using reference = T &;
	using pointer = T *;
	using difference_type = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	fwd_list_iterator() : _node{nullptr} {}
	explicit fwd_list_iterator(fwd_list_node<T> * n) : _node{n} {}

	reference operator*() const {return _node->value;}
	pointer operator->() const {return &_node->value;}

	self & operator++()  // pre increment
	{
		_node = _node->next;
		return *this;
	}

	self operator++(int)  // post increment
	{
		self tmp{*this};
		_node = _node->next;
		return tmp;
	}

	bool operator==(self const & rhs) const {return _node == rhs._node;}
	bool operator!=(self const & rhs) const {return _node != rhs._node;}

	fwd_list_node<T> * _node;
};

template <typename T>
struct fwd_list_const_iterator
{
	using self = fwd_list_const_iterator<T>;
	using iterator = fwd_list_iterator<T>;
	using value_type = T;
	using pointer = T const *;
	using reference = T const &;
	using difference_type = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	fwd_list_const_iterator() : _node{nullptr} {}
	explicit fwd_list_const_iterator(fwd_list_node<T> * n) : _node{n} {}
	fwd_list_const_iterator(iterator const & it) : _node{it._node} {}

	reference operator*() const {return _node->value;}
	pointer operator->() const {return &_node->value;}

	self & operator++()  // pre increment
	{
		_node = _node->next;
		return *this;
	}

	self operator++(int)  // post increment
	{
		self tmp{*this};
		_node = _node->next;
		return tmp;
	}

	bool operator==(self const & rhs) const {return _node == rhs._node;}
	bool operator!=(self const & rhs) const {return _node != rhs._node;}

	fwd_list_node<T> * _node;
};

template <typename T>
class forward_list
{
public:
	using node = fwd_list_node<T>;
	using iterator = fwd_list_iterator<T>;
	using const_iterator = fwd_list_const_iterator<T>;
	using reference = T &;
	using const_reference = T const &;

	forward_list();
	~forward_list();
	reference front();
	const_reference front() const;
	iterator begin() {return iterator{_head};}
	const_iterator begin() const {return const_iterator{_head};}
	iterator end() {return iterator{};}
	const_iterator end() const {return const_iterator{};}
	bool empty() const;
	size_t size() const;
	void clear();
	iterator insert_after(const_iterator pos, T const & value);
	iterator erase_after(const_iterator pos);
	void push_front(T const & value);
	void pop_front();
	void swap(forward_list & other);

private:
	size_t _count;
	node * _head;
};

template <typename T>
forward_list<T>::forward_list() : _count{0}, _head{nullptr}
{}

template <typename T>
forward_list<T>::~forward_list()
{
	clear();
}

template <typename T>
typename forward_list<T>::reference forward_list<T>::front()
{
	return _head->value;
}

template <typename T>
typename forward_list<T>::const_reference forward_list<T>::front() const
{
	return _head->value;
}

template <typename T>
bool forward_list<T>::empty() const
{
	return _count == 0;
}

template <typename T>
size_t forward_list<T>::size() const
{
	return _count;
}

template <typename T>
void forward_list<T>::clear()
{
	for (node * n = _head; n; n = n->next)
		pop_front();
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::insert_after(const_iterator pos, T const & value)
{
	node * n = new node{};
	n->value = value;
	n->next = pos._node->next;
	pos._node->next = n;
	++_count;
	return iterator{n};
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::erase_after(const_iterator pos)
{
	node * curr = pos._node->next;
	pos._node->next = curr->next;
	delete curr;
	--_count;
	return iterator{pos._node->next};
}

template <typename T>
void forward_list<T>::push_front(T const & value)
{
	node * n = new node{};
	n->value = value;
	n->next = _head;
	_head = n;
	++_count;
}

template <typename T>
void forward_list<T>::pop_front()
{
	assert(!empty() && "snazim sa vymazat prvok s prazdneho zoznamu");
	node * n = _head;
	_head = n->next;
	delete n;
	--_count;
}

template <typename T>
void forward_list<T>::swap(forward_list & other)
{
	std::swap(_count, other._count);
	std::swap(_head, other._head);
}
