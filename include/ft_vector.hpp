// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP


// --------------------------------------------------------------------------------------------------------------------
// [참고 0. std::vector ]
 #include <vector>
 #include <list>

// [참고 1. Cherno ] 		: https://www.youtube.com/watch?v=ryRf4Jh_YC0&t=1354s
// [참고 2. gcc-mirror ]	    : https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h
// [참고 3. C++ Standard ] 	: https://www.lirmm.fr/~ducour/Doc-objets/ISO+IEC+14882-1998.pdf
// [참고 4. First STL ] 	    : stepanov 1995 소스코드
// --------------------------------------------------------------------------------------------------------------------

#include <memory>
#include "__config.hpp"
#include "ft_utility.hpp"
#include "ft_iterator.hpp"

FT_BEGIN_GLOBAL_NAMESPACE

template<typename T, class Allocator = std::allocator<T> >
class vector
{
public:// typedefs
	typedef T                                           value_type;
	typedef Allocator                                   allocator_type;
	typedef typename allocator_type::size_type          size_type;          // size_t n std::allocator
	typedef typename allocator_type::difference_type    difference_type;    // ptrdiff_t on std::allocator
	typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
	typedef typename allocator_type::pointer            pointer;            // _Tp* on std::allocator
	typedef typename allocator_type::const_pointer      const_pointer;      // const _Tp* on std::allocator

	// Subject : If the container has an iterator system, you must implement it.
	typedef typename FT::random_access_iterator<std::random_access_iterator_tag, pointer>            iterator;
	typedef typename FT::random_access_iterator<std::random_access_iterator_tag, const_pointer>      const_iterator;
	typedef typename FT::reverse_iterator<iterator>			                                         reverse_iterator;
	typedef typename FT::reverse_iterator<const_iterator>                                            const_reverse_iterator;

protected: // allocator instance
	allocator_type m_Allocator;

private: // data members
	iterator m_Start;           // begin
	iterator m_Finish;          // end
	iterator m_End_of_storage;  // end of memory (capacity)

private: // helper functions

	// destruct every object from start to end.
	FT_INLINE_VISIBILITY
	void _destroy(iterator start, iterator end) FT_NOEXCEPT
	{
		while (start != end)
		{
			m_Allocator.destroy(start);
			start++;
		}
	}

	// data reallocation to new block of memory (force change)
	FT_INLINE_VISIBILITY
	void _reAlloc(size_t newCapacity) FT_NOEXCEPT
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
	FT_INLINE_VISIBILITY
	explicit vector(const allocator_type& _allocator = allocator_type()) FT_NOEXCEPT
		: m_Allocator(_allocator), m_Start(0), m_Finish(0), m_End_of_storage(0) // init memnber to 0
	{}

	/* Constructs a container with n elements. Each element is a copy of val. */
	FT_ALWAYS_INLINE
	explicit vector(size_type n, const T &value = T(), const allocator_type& _allocator = allocator_type())
	{
		// * 이렇게 호출해도 되는겨? Allocator instance가 없는데?
		m_Start = _allocator.allocate(n);
		std::uninitialized_fill_n(m_Start, n, value); // using function at <memory.h>, Cpp98
		m_Finish = m_Start + n;
		m_End_of_storage = m_Finish;
	}

	// https://cplusplus.com/reference/vector/vector/vector/
	// copy constructor via other vector's iterator
	// [ * Member Function Re-templatize ]
	template <class InputIterator>
	FT_INLINE_VISIBILITY
	vector(InputIterator first, InputIterator last, const allocator_type& _allocator = allocator_type()) FT_NOEXCEPT
		: m_Allocator(_allocator), m_Start(0), m_Finish(0), m_End_of_storage(0) // init memnber to 0
	{
		const difference_type new_size = std::distance(first, last);
		if (new_size > 0) {
			m_Start = m_Allocator.allocate(new_size);
			m_Finish = std::uninitialized_copy(first, last, m_Start);
			m_End_of_storage = m_Finish;
		}
	}

	// copy constructor via other vector
	FT_INLINE_VISIBILITY
	vector(const FT::vector<T, allocator_type> &x) FT_NOEXCEPT
		: m_Allocator(allocator_type())
	{
		const difference_type new_size = std::distance(x.begin(), x.end());
		m_Start = m_Allocator.allocate(new_size);
		m_Finish = std::uninitialized_copy(x.begin(), x.end(), m_Start);
		m_End_of_storage = m_Finish;
	}

	~vector()
	{
		this->_destroy(m_Start, m_Finish);
		m_Allocator.deallocate(m_Start, this->capacity());
	}

	vector<T, allocator_type>& operator=(const FT::vector<T, allocator_type> &other)
	{
		// if self assignment
		if (*this == other) return *this;

		// if other is larger, then need to reallocate memory
		this->_destroy(m_Start, m_Finish);
		if (other.size() > this->capacity())
		{
			m_Allocator.deallocate(m_Start, this->capacity());
			const difference_type new_size = std::distance(other.begin(), other.end());
			m_Start = this->m_Allocator.allocate(new_size);
			m_Finish = std::uninitialized_copy(other.begin(), other.end(), m_Start);
			m_End_of_storage = m_Finish;
		}
		else
			m_Finish = std::uninitialized_copy(other.begin(), other.end(), m_Start);

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

	// ---------------------------------------------------------------------------
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

	void push_back(const T &value)
	{
		if (this->size() >= this->capacity()) {
			_reAlloc(this->capacity() + (this->capacity() / 2));
		}
		m_Allocator.construct(m_Finish, value);
		m_Finish++;
	}

	void pop_back()
	{
		if (this->empty()) {
			return ;
		}
		m_Allocator.destory(--m_Finish);
	}

	// TODO: 이건 성능 검증을 좀 해야 겠다.
	iterator insert(iterator position, const T &value)
	{
		// 1 2 3 .

		//   p   c
		// 1 2 3 A .

		//   p c
		// 1 2 A 3 .

		//   c
		// 1 A 2 3 .

		push_back(value); // (1) 일단 맨 뒤에 넣고.
		iterator cur = m_Finish - 1; // cur = 마지막 원소
		while (cur != position) {
			// (2) 뒤에서 앞으로 swap 진행.
			_PRIVATE::swap(*(cur), *(--cur));
		}
		return (cur);
	}

	// TODO: 굳이 destroy 를 해야 하나? 그냥 덮어 쓰면 안되나...
	void insert(iterator position, size_type n, const T &value)
	{
		// (1) 뒷 부분 따로 보유.
		FT::vector<T> tmp(position, m_Finish);
		_destroy(position, m_Finish);
		// (2) 공간 필요시 확장.
		if (this->size() + n >= this->capacity()) {
			_reAlloc(this->capacity() + (this->capacity() / 2) + n);
		}
		// (3) position 부터 value n개 삽입.
		std::uninitialized_fill_n(position, n, value);
		// (4) position + n 부터 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), position + n);
		// (5) 백업본 삭제.
		_destroy(tmp.begin(), tmp.end());
		m_Allocator.deallocate(tmp.begin(), tmp.capacity());
	}

	// TODO: Possible refactoring !
	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		// (0) first 부터 last 까지 개수 구하기
		const difference_type sizeToCopy = std::distance(first, last);
		// (1) 뒷 부분 따로 보유.
		FT::vector<T> tmp(position, m_Finish);
		_destroy(position, m_Finish);
		// (2) 공간 필요시 확장.
		if (this->size() + sizeToCopy >= this->capacity()) {
			_reAlloc(this->capacity() + (this->capacity() / 2) + sizeToCopy);
		}
		// (3) position 부터 value n개 삽입.
		m_Finish = std::uninitialized_copy(first, last, position);
		// (4) position + sizeToCopy 부터 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), position + sizeToCopy);
		// (5) 백업본 삭제.
		_destroy(tmp.begin(), tmp.end());
		m_Allocator.deallocate(tmp.begin(), tmp.capacity());
	}

	/* An iterator pointing to the new location of the element
	that followed the last element erased by the function call. */
	iterator erase(iterator position)
	{
		//   c
		// 1 A 2 3 .

		//   p c
		// 1 2 A 3 .

		//   p   c
		// 1 2 3 A .

		//   p   c
		// 1 2 3 x

		iterator cur = position;
		while (cur != (m_Finish - 1)) {
			_PRIVATE::swap(*cur, *(++cur));
		}
		pop_back();
		return (position + 1);
	}

	/* An iterator pointing to the new location of the element
	that followed the last element erased by the function call. */
	iterator erase(iterator first, iterator last)
	{
		if (first == last)
			return ;
		// (0) first 부터 last 까지 개수 구하기
		const difference_type sizeToReduce = std::distance(first, last);
		// (1) 삭제 뒷 부분 따로 보유.
		FT::vector<T> tmp(last, m_Finish);
		// (2) first 이후 부터 싹 다 제거.
		_destroy(first, m_Finish);
		// (3) first 로 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), m_Start);
		// (4) 백업본 삭제.
		_destroy(tmp.begin(), tmp.end());
		m_Allocator.deallocate(tmp.begin(), tmp.capacity());
	}


	void swap(FT::vector<T, allocator_type>& other)
	{
		// 두 벡터간 데이터 교체가 iterator만 교체해주면 되기 때문에 몹시 쉬움.
		_PRIVATE::swap(m_Start, other.m_Start);
		_PRIVATE::swap(m_Finish, other.m_Finish);
		_PRIVATE::swap(m_End_of_storage, other.m_End_of_storage);
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
// -----------------------------------------------------------

template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator==(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return x.size() == y.size() && _PRIVATE::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator!=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(x == y);
}

template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator< (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return _PRIVATE::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}


template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator> (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return y < x;
}

template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator>=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(x < y);
}

template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
bool operator<=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(y < x);
}

// specialized algorithms:
// TODO: Throw() 를 함수 옆에 했을 때 어떤 변화가 일어나는지 공부할 것
template <class T, class Allocator>
inline FT_INLINE_VISIBILITY
void swap(FT::vector<T,Allocator>& x, FT::vector<T,Allocator>& y)
	FT_NOEXCEPT_(FT_NOEXCEPT_(x.swap(y)))
{
	x.swap(y);
}

FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER_VECTOR_HPP