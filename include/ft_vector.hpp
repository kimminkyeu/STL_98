// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP

#include <memory>
//#include <algorithm>
#include <map>

#include "__config.hpp"
#include "ft_utils.hpp"
#include "ft_iterator.hpp"

// [참고 0. std::vector ]
 #include <vector>

// [참고 1. Cherno ] 		: https://www.youtube.com/watch?v=ryRf4Jh_YC0&t=1354s
// [참고 2. gcc-mirror ]	    : https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h
// [참고 3. C++ Standard ] 	: https://www.lirmm.fr/~ducour/Doc-objets/ISO+IEC+14882-1998.pdf 
// [참고 4. First STL ] 	    : stepanov 1995 소스코드

namespace ft
{




template<typename T, class Allocator = std::allocator<T> >
class vector
{

public: // typedefs
	typedef T											value_type;
	typedef Allocator							        allocator_type;
	typedef typename allocator_type::size_type			size_type;
	typedef typename allocator_type::difference_type	difference_type; // ? 이건 뭐지 ?
	typedef typename allocator_type::reference       	reference;
    typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;


	// TODO: need to implement iterator class for vector?
	typedef typename allocator_type::pointer			iterator; 		// ? 이렇게 해도 되나 ?
	typedef typename allocator_type::const_pointer		const_iterator; // ? 이렇게 해도 되나 ?

	// TODO: ??
	typedef typename ft::reverse_iterator<iterator>				reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator>		const_reverse_iterator;


	// ? 1995 소스코드를 보면 static으로 되어 있다. 이게 왜 static인지 모르겠다. 그리고 그게 좋나?
private:
	allocator_type m_Allocator;

private: // data members
	iterator m_Start;           // begin
	iterator m_Finish;          // end
	iterator m_End_of_storage;  // end of memory

private: // helper functions

	// destruct every object from start to end.
	_FT_INLINE_VISIBILITY
	void _destroy(iterator start, iterator end) _FT_NOEXCEPT
	{
		while (start != end)
		{
			m_Allocator.destroy(start);
			start++;
		}
	}

	// data reallocation to new block of memory (force change)

	// ! 근데 만약 벡터에 객체 포인터가 들어있으면 어떻게 되나?
	_FT_INLINE_VISIBILITY
	void _reAlloc(size_t newCapacity) _FT_NOEXCEPT
	{
		if (newCapacity == this->capacity())
			return;

		// 1. allocate a new block of memory
		iterator new_start = m_Allocator.allocate(newCapacity);

		// 2. copy/move old elements into new block
		size_type diff = 0;
		if (newCapacity < this->size()) // if reallocate to smaller block
			diff = this->size() - newCapacity;
		iterator new_finish = std::uninitialized_copy(m_Start, m_Finish - diff, new_start);

		// 3. delete original & change m_iterator to point new block
		this->_destroy(m_Start, m_Finish);
		m_Allocator.deallocate(m_Start, this->capacity());
		m_Start = new_start;
		m_Finish = new_finish;
	}

public:
	// 23.2.4.1 construct/copy/destroy:
	// ---------------------------------------------------------------------------

	/* Constructs an empty container, with no elements */
	_FT_INLINE_VISIBILITY
	explicit vector(const allocator_type& _allocator = allocator_type()) _FT_NOEXCEPT
		: m_Allocator(_allocator), m_Start(0), m_Finish(0), m_End_of_storage(0) // init memnber to 0
	{}

	/* Constructs a container with n elements. Each element is a copy of val. */
	_FT_ALWAYS_INLINE
	explicit vector(size_type n, const T &value = T(), const allocator_type& _allocator = allocator_type())
	{
		// ! 이렇게 호출해도 되는겨? Allocator instance가 없는데?
		m_Start = _allocator.allocate(n);
		std::uninitialized_fill_n(m_Start, n, value); // using function at <memory.h>, Cpp98
		m_Finish = m_Start + n;
		m_End_of_storage = m_Finish;
	}

	// https://cplusplus.com/reference/vector/vector/vector/
	// copy constructor via other vector's iterator
	// [ * Member Function Re-templatize ]

	template <class InputIterator>
	_FT_INLINE_VISIBILITY
	vector(InputIterator first, InputIterator last, const allocator_type& _allocator = allocator_type()) _FT_NOEXCEPT
		: m_Allocator(_allocator)
	{
		const difference_type new_size = std::distance(first, last);
		m_Start = m_Allocator.allocate(new_size);
		m_Finish = std::uninitialized_copy(first, last, m_Start);
		m_End_of_storage = m_Finish;
	}

	// copy constructor via other vector
	_FT_INLINE_VISIBILITY
	vector(const ft::vector<T, allocator_type> &x) _FT_NOEXCEPT
		: m_Allocator(allocator_type())
	{
		m_Start = m_Allocator.allocate(x.end() - x.begin());
		m_Finish = std::uninitialized_copy(x.begin(), x.end(), m_Start);
		m_End_of_storage = m_Finish;
	}

	~vector()
	{
		this->_destroy(m_Start, m_Finish);
		m_Allocator.deallocate(m_Start, this->capacity());
	}

	vector<T, allocator_type>& operator=(const vector<T, allocator_type> &other)
	{
		// if self assignment
		if (*this == other) return *this;

		// if other is larger, then need to reallocate memory
		if (other.size() > this->capacity())
		{
			// (1) delete original
			this->_destroy(m_Start, m_Finish);
			m_Allocator.deallocate(m_Start, this->capacity());

			// (2) copy
			m_Start = this->m_Allocator.allocate(other.end() - other.begin());
			m_End_of_storage = std::uninitialized_copy(other.begin(), other.end(), m_Start);
		}
		// ? if self is larger than other ?
		else if (this->size() > other.size())
		{

		}
		else
		{

		}

		return (*this);
	}

	/* Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
	If a reallocation happens,the storage needed is allocated using the internal allocator. */

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		this->clear();
		const difference_type new_size = std::distance(first, last);
		if (new_size > this->capacity())
			_reAlloc(new_size);
		m_Finish = std::uninitialized_copy(first, last, m_Start);
	}

	void assign(size_type n, const T &value)
	{
		// 벡터 객체에 이전에 있었던 원소들은 모두 삭제하고, 인자로 받은 새로운 내용을 집어 넣는다. 원래 내용을 다 지우고 원소 u 를 n 개 가지는 벡터로 만든다.
		this->clear();
		if (n > this->capacity())
			_reAlloc(n);
		std::uninitialized_fill_n(m_Start, n, value);
		m_Finish = m_Start + n;
	}

	allocator_type get_allocator() const
	{
		return m_Allocator;
	}

	// ---------------------------------------------------------------------------
	// iterators:

	iterator begin() { return m_Start; }

	const_iterator begin() const { return m_Start; }

	iterator end() { return m_Finish; }

	const_iterator end() const { return m_Finish; }

	reverse_iterator rbegin() { return reverse_iterator(end()); }

	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }

	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	// 23.2.4.2 capacity:

	/* Returns the number of elements in the vector. */
	size_type size() const { return size_type(end() - begin()); }

	/* Returns the maximum object size supported by the allocator */
	size_type max_size() const { return m_Allocator.max_size(); }

	/* Returns the size of the storage space currently allocated for the vector, expressed in terms of elements. */
	size_type capacity() const { return m_End_of_storage - m_Start; }

	bool empty() const { return ( begin() == end()); }

	/*
	Resizes the container so that it contains n elements.
	If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
	If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n. If val is specified, the new elements are initialized as copies of val, otherwise, they are value-initialized.
	If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
	Notice that this function changes the actual content of the container by inserting or erasing elements from it.	*/
	void resize(size_type n, T value = T())
	{
		if (n == this->size())
			return ;
		else if (n < this->size())
			_destroy(m_Start + n, m_Finish);
		else // n > this->size()
		{
			if (n > this->capacity()) // if n is also greater than capacity
				_reAlloc(n);
			std::uninitialized_fill_n(m_Finish, n - m_Finish, value);
		}
		m_Finish = m_Start + n;
	}

	/*
	Requests that the vector capacity be at least enough to contain n elements.
	If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
	In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
	This function has no effect on the vector size and cannot alter its elements. */
	void reserve(size_type n)
	{
		if (n > this->capacity())
			_reAlloc(n);
	}

	// ---------------------------------------------------------------------------
	// element access:

	reference operator[](size_type n)
	{
		if (n > this->size())
		{
			// handle error...
		}
		return *(begin() + n);
	}

	const_reference operator[](size_type n) const
	{
		if (n > this->size())
		{
			// handle error...
		}
		return *(begin() + n);
	}

	const_reference at(size_type n) const { return *(begin() + n); }

	reference at(size_type n) { return *(begin() + n); }

	reference front() { return *begin(); }

	const_reference front() const { return *begin(); }

	reference back() { return *(--end()); }

	const_reference back() const { return *(--end()); }

	// ---------------------------------------------------------------------------
	// 23.2.4.3 modifiers:

	void push_back(const T &x)
	{

	}

	void pop_back()
	{

	}

	iterator insert(iterator position, const T &x)
	{

	}

	void insert(iterator position, size_type n, const T &x)
	{

	}

	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{

	}

	iterator erase(iterator position)
	{

	}

	iterator erase(iterator first, iterator last)
	{

	}


	void swap(vector<T, allocator_type> & other)
	{
		// 두 벡터간 데이터 교체가 iterator만 교체해주면 되기 때문에 몹시 쉬움.
		ft::swap(m_Start, other.m_Start);
		ft::swap(m_Finish, other.m_Finish);
		ft::swap(m_End_of_storage, other.m_End_of_storage);
	}

	// clear : 원소를 모두 제거 한다.
	void clear()
	{
		// size()가 0으로 바뀌고, 기존 원소들은 삭제.
		this->_destroy(m_Start, m_Finish);
		m_Finish = m_Start;
	}






};

// Non-member function.
// 왜 Non-member인지는 객체 지향에 대한 이해가 필요하다.
// 참고링크 : https://www.codeproject.com/Articles/4534/Non-Member-Functions-in-OOP
// 참고링크 : https://www.aristeia.com/Papers/CUJ_Feb_2000.pdf --> effective c++ 작성자의 글

// TODO: ft::equal() 과 ft::lexicographical_compare() 구현하기.

template <class T>
bool operator== (const vector<T>& x, const vector<T>& y)
{
	return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}

template <class T>
bool operator< (const vector<T>& x, const vector<T>& y)
{
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}




} // namespace ft

#endif //FT_CONTAINER_VECTOR_HPP
