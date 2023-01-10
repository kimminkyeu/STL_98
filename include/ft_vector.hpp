// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP

#include <memory>
#include "__config.hpp"
#include "ft_type_traits.hpp"
#include "ft_utility.hpp"
#include "ft_iterator.hpp"

FT_BEGIN_GLOBAL_NAMESPACE

template <class Tp, class Allocator>
class Vector_alloc_base
{
public: // typedefs
    typedef Allocator   allocator_type;

    explicit Vector_alloc_base(const allocator_type& _a)
            : m_Data_allocator(_a), m_Start(0), m_Finish(0), m_End_of_storage(0)
    {}

protected:
    // ------------------------------
    allocator_type m_Data_allocator;
    // ------------------------------
    Tp* m_Start;
    Tp* m_Finish;
    Tp* m_End_of_storage;

    Tp* m_Allocate(size_t _n)
    {
        return m_Data_allocator.allocate(_n);
    }

    void m_Deallocate(Tp* _p, size_t _n)
    {
        if (_p) { m_Data_allocator.deallocate(_p, _n); }
    }

public:
    allocator_type get_allocator() const
    {
        return m_Data_allocator;
    }
};


// ---------------------------------------------------------------
// |                                                             |
// |                Vector-Base implementation                   |
// |                                                             |
// ---------------------------------------------------------------
// The vector base class serves two purposes.
// First, its constructor and destructor allocate (but don't initialize) storage. This makes exception safety easier.
// Second, the base class encapsulates all the differences between SGI-style allocators and standard-conforming allocators.
// An SGI STL allocator consists of a class with 3 required member functions, all of which are static.
// (SGI는 boost library의 allocator 디자인으로, 메모리 할당/해제 함수가 static 이다.)
// Ref : https://www.boost.org/sgi/stl/alloc.html

template <class Tp, class Alloc>
class Vector_base : public Vector_alloc_base<Tp, Alloc>
{
protected: // typedef and namespace scope
    typedef Vector_alloc_base<Tp, Alloc>                       _Vector_alloc_base;
    typedef typename _Vector_alloc_base::allocator_type        allocator_type;
    using _Vector_alloc_base::m_Finish;
    using _Vector_alloc_base::m_Start;
    using _Vector_alloc_base::m_End_of_storage;
    using _Vector_alloc_base::m_Allocate;
    using _Vector_alloc_base::m_Deallocate;

public:
    explicit Vector_base(const allocator_type& _a)
            : _Vector_alloc_base(_a)
    {}

    explicit Vector_base(size_t _n, const allocator_type& _a)
            : _Vector_alloc_base(_a)
    {
        m_Start = m_Allocate(_n);
        m_Finish = m_Start;
        m_End_of_storage = m_Start + _n;
    }

    ~Vector_base()
    {
        m_Deallocate(m_Start, m_End_of_storage - m_Start);
    }
};


// ---------------------------------------------------------------
// |                                                             |
// |                   Vector implementation                     |
// |                                                             |
// ---------------------------------------------------------------

template<typename T, class Allocator = std::allocator<T> >
class vector : protected Vector_base<T, Allocator>
{
private:
    typedef Vector_base<T, Allocator>			        _Vector_base;
    using _Vector_base::Vector_alloc_base::m_Finish;
    using _Vector_base::Vector_alloc_base::m_Start;
    using _Vector_base::Vector_alloc_base::m_End_of_storage;
    using _Vector_base::Vector_alloc_base::m_Allocate;
    using _Vector_base::Vector_alloc_base::m_Deallocate;

private:
    typedef vector<T, Allocator>    					vector_type;

public:
    typedef T                                           value_type;
    // same as FT::allocator_traits<Tp, Allocator>::allocator_type
	typedef typename _Vector_base::allocator_type		allocator_type;
    typedef typename allocator_type::size_type          size_type;          // size_t n std::allocator
    typedef typename allocator_type::difference_type    difference_type;    // ptrdiff_t on std::allocator
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::pointer            pointer;            // _Tp* on std::allocator
	typedef typename allocator_type::const_pointer      const_pointer;      // const _Tp* on std::allocator
    typedef typename FT::random_access_iterator<pointer, vector_type>            				iterator;
    typedef typename FT::random_access_iterator<const_pointer, vector_type>     				const_iterator;
    typedef typename FT::reverse_iterator<iterator>			                                    reverse_iterator;
	typedef typename FT::reverse_iterator<const_iterator>                                       const_reverse_iterator;

private: // helper functions
	// data reallocation to new block of memory (force change)
	void _reAlloc(size_t newCapacity)
	{
		if (newCapacity == capacity())
			return;

		// 1. allocate a new block of memory
		// iterator new_start = m_Allocator.allocate(newCapacity);
		pointer new_start = m_Allocate(newCapacity);

		// 2. copy/move old elements into new block
		size_type diff = 0;
		if (newCapacity < size()) // if reallocate to smaller block
			diff = size() - newCapacity;
		pointer new_finish = std::uninitialized_copy(m_Start, m_Finish - diff, new_start);

		// 3. delete original & change m_iterator to point new block
		_PRIVATE::destroy(m_Start, m_Finish);
        m_Deallocate(m_Start, this->capacity());
		m_Start = new_start;
		m_Finish = new_finish;
		m_End_of_storage = new_start + newCapacity;
	}

public:
	// 23.2.4.1 construct/copy/destroy:
	// ---------------------------------------------------------------------------
	/* Constructs an empty container, with no elements */

	explicit vector(const allocator_type& _allocator = allocator_type())
		: _Vector_base(_allocator)
	{}

	/* Constructs a container with n elements. Each element is a copy of val. */
	explicit vector(size_type _n, const T &_value = T(), const allocator_type& _allocator = allocator_type())
		: _Vector_base(_n, _allocator)
	{
		std::uninitialized_fill_n(m_Start, _n, _value); // using function at <memory.h>, Cpp98
		m_Finish = m_Start + _n;
	}

	// * enable_if 는 여기에서 필요하다. 왜냐하면 std::vector<int>(10, 20)이 어떤 생성자인지 모르기 때문이다.
    // * vector에서 iterator는 포인터이고, 주소값을 가지기에 integral 범위 내에선 위 두번째 생성자와 구분할 수 있는 기준이 없다.
    // * 따라서 컴파일 에러가 발생한다.
	// https://cplusplus.com/reference/vector/vector/vector/
	// copy constructor via other vector's iterator
	// [ * Member Function Re-templatize ]
	template <class InputIterator>
	vector(InputIterator first, InputIterator last, const allocator_type& _allocator = allocator_type(),
				typename FT::enable_if<!(FT::is_integral<InputIterator>::value)>::type * = 0)
				// if not integral, then value is false.
		: _Vector_base(std::distance(first, last), _allocator)
	{
		m_Finish = std::uninitialized_copy(first, last, m_Start);
	}

	// copy constructor via other vector
	explicit vector(const FT::vector<T, allocator_type> &x)
		: _Vector_base(x.size(), x.get_allocator())
	{
		m_Finish = std::uninitialized_copy(x.begin(), x.end(), m_Start);
	}

	~vector()
	{
		_PRIVATE::destroy(m_Start, m_Finish);
	}

    vector_type& operator=(const vector_type& other)
	{
		// if self assignment
		if (*this == other) return *this;

		// if other is larger, then need to reallocate memory
        _PRIVATE::destroy(m_Start, m_Finish);
		if (other.size() > this->capacity())
		{
			const difference_type new_size = std::distance(other.begin(), other.end());

			m_Deallocate(m_Start, this->capacity());
			m_Start = m_Allocate(new_size);
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
		m_Finish = this->_Start + n;
	}

	// ---------------------------------------------------------------------------
	// iterators:

	iterator begin() { return iterator(m_Start); }

	const_iterator begin() const { return const_iterator(m_Start); }

	iterator end() { return iterator(m_Finish); }

	const_iterator end() const { return const_iterator(m_Finish); }

	reverse_iterator rbegin() { return reverse_iterator(end()); }

	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

	reverse_iterator rend() { return reverse_iterator(begin()); }

	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	// ---------------------------------------------------------------------------
	// 23.2.4.2 capacity:

	/* Returns the number of elements in the vector. */
	size_type size() const { return size_type(end() - begin()); }

	/* Returns the maximum object size supported by the allocator */
	size_type max_size() const {
		// NOTE:  do not use allocator's max_size(), because it's optinal function call
		//  return m_Allocator.max_size();
		//  REF (1) : https://en.cppreference.com/w/cpp/memory/allocator/max_size
		//  REF (2) : https://en.cppreference.com/w/cpp/named_req/Allocator --> optional.

		// from max_size cpp reference
		return (std::numeric_limits<size_type>::max() / sizeof(value_type));
	}

	/* Returns the size of the storage space currently allocated for the vector, expressed in terms of elements. */
	// size_type capacity() const { return m_End_of_storage - m_Start; }
	size_type capacity() const
	{
		return m_End_of_storage - m_Start;
	}

	bool empty() const { return ( begin() == end()); }

	/*
	Resizes the container so that it contains n elements.
	If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
	If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n. If val is specified, the new elements are initialized as copies of val, otherwise, they are value-initialized.
	If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
	Notice that this function changes the actual content of the container by inserting or erasing elements from it.	*/
	void resize(size_type _n, T _value = T())
	{
		if (_n == this->size())
			return ;
		else if (_n < this->size())
			_PRIVATE::destroy(m_Start + _n, m_Finish);
		else // n > this->size()
		{
			if (_n > this->capacity()) // if n is also greater than capacity
				_reAlloc(_n);
			std::uninitialized_fill_n(m_Finish, _n - m_Finish, _value);
		}
		m_Finish = m_Start + _n;
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
			_reAlloc(this->capacity() * 2);
		}
        _PRIVATE::construct(m_Finish, value);
		++m_Finish;
	}

	void pop_back()
	{
		if (this->empty()) {
			return ;
		}
		// m_Allocator.destory(--m_Finish);
		_PRIVATE::destroy(--m_Finish);
	}

	// TODO: 이건 성능 검증을 좀 해야 겠다.
	iterator insert(iterator _position, const T& _value)
	{
		// 1 2 3 .

		//   p   c
		// 1 2 3 A .

		//   p c
		// 1 2 A 3 .

		//   c
		// 1 A 2 3 .

		/* --> 원래 아래 코드로 짰으나, 비효율적인 코드
		push_back(value); // (1) 일단 맨 뒤에 넣고.
		iterator cur = m_Finish - 1; // cur = 마지막 원소
		while (cur != position) {
			// (2) 뒤에서 앞으로 swap 진행.
			_PRIVATE::swap(*(cur), *(--cur));
		}
		return (cur);
		*/
		if (this->size() >= this->capacity()) {
			_reAlloc(this->capacity() * 2);
		}
		++m_Finish;
		std::copy_backward(_position, end() - 2, end() - 1);
		*_position = _value;
	}

	// TODO: 굳이 destroy 를 해야 하나? 그냥 덮어 쓰면 안되나...
	void insert(iterator position, size_type n, const T &value)
	{
		// (1) 뒷 부분 따로 보유.
		FT::vector<T> tmp(position, m_Finish);
		_PRIVATE::destroy(position, m_Finish);
		// (2) 공간 필요시 확장.
		if (this->size() + n >= this->capacity()) {
			_reAlloc((this->capacity() * 2) + n);
		}
		// (3) position 부터 value n개 삽입.
		std::uninitialized_fill_n(position, n, value);
		// (4) position + n 부터 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), position + n);
		// (5) 백업본 삭제.
		_PRIVATE::destroy(tmp.begin(), tmp.end());
		m_Deallocate(tmp.begin(), tmp.capacity());
	}

	// TODO: Possible refactoring !
	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		// (0) first 부터 last 까지 개수 구하기
		const difference_type sizeToCopy = std::distance(first, last);
		// (1) 뒷 부분 따로 보유.
		FT::vector<T> tmp(position, m_Finish);
		_PRIVATE::destroy(position, m_Finish);
		// (2) 공간 필요시 확장.
		if (this->size() + sizeToCopy >= this->capacity()) {
			_reAlloc((this->capacity() * 2) + sizeToCopy);
		}
		// (3) position 부터 value n개 삽입.
		m_Finish = std::uninitialized_copy(first, last, position);
		// (4) position + sizeToCopy 부터 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), position + sizeToCopy);
		// (5) 백업본 삭제.
		_PRIVATE::destroy(tmp.begin(), tmp.end());
		m_Deallocate(tmp.begin(), tmp.capacity());
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
			FT::swap(*cur, *(++cur));
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
		_PRIVATE::destroy(first, m_Finish);
		// (3) first 로 백업본 복사.
		m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), m_Start);
		// (4) 백업본 삭제.
		_PRIVATE::destroy(tmp.begin(), tmp.end());
		m_Deallocate(tmp.begin(), tmp.capacity());
	}


	void swap(FT::vector<T, allocator_type>& other)
	{
		// 두 벡터간 데이터 교체가 iterator만 교체해주면 되기 때문에 몹시 쉬움.
		FT::swap(m_Start, other.m_Start);
		FT::swap(m_Finish, other.m_Finish);
		FT::swap(m_End_of_storage, other.m_End_of_storage);
	}

	// clear : 원소를 모두 제거 한다.
	void clear()
	{
		// size()가 0으로 바뀌고, 기존 원소들은 삭제.
		_PRIVATE::destroy(m_Start, m_Finish);
		m_Finish = m_Start;
	}
};

// Non-member function.
// -----------------------------------------------------------

template <class T, class Allocator>
inline
bool operator==(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return x.size() == y.size() && FT::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
inline
bool operator!=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(x == y);
}

template <class T, class Allocator>
inline
bool operator< (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return FT::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}


template <class T, class Allocator>
inline
bool operator> (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return y < x;
}

template <class T, class Allocator>
inline
bool operator>=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(x < y);
}

template <class T, class Allocator>
inline
bool operator<=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
	return !(y < x);
}

// specialized algorithms:
// TODO: Throw() 를 함수 옆에 했을 때 어떤 변화가 일어나는지 공부할 것
template <class T, class Allocator>
inline
// 조건부 noexcept
void swap(FT::vector<T,Allocator>& x, FT::vector<T,Allocator>& y)
{
	x.swap(y);
}


FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER_VECTOR_HPP
