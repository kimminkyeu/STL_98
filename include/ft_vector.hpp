// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP

#include <memory>

// [참고 0. std::vector ]
// #include <vector>

// [참고 1. Cherno ] 		: https://www.youtube.com/watch?v=ryRf4Jh_YC0&t=1354s
// [참고 2. gcc-mirror ]	: https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_vector.h
// [참고 3. C++ Standard ] 	: https://www.lirmm.fr/~ducour/Doc-objets/ISO+IEC+14882-1998.pdf 
// [참고 4. First STL ] 	: stepanov 1995 소스코드

namespace ft
{


template<typename T, class Allocator = std::allocator<T> >
class vector
{
	


public: // typedefs

	typedef T											value_type;
	typedef Allocator									allocator_type;
	typedef typename allocator_type::size_type			size_type;
	typedef typename allocator_type::difference_type	difference_type; // ? 이건 뭐지 ?
	typedef typename allocator_type::reference       	reference;
    typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;
	typedef typename allocator_type::pointer			iterator; 		// ? 이렇게 해도 되나 ? 
	typedef typename allocator_type::const_pointer		const_iterator; // ? 이렇게 해도 되나 ?
	// TODO: need to implement iterator_traits + reverse_iterator
	// typedef ft::reverse_iterator<iterator>				reverse_iterator;
	// typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;


private: // data members
	iterator m_Start; // begin
	iterator m_Finish; // end
	iterator m_End_of_storage; // end of memory


public:
	// 23.2.4.1 construct/copy/destroy:
	// ---------------------------------------------------------------------------
	explicit vector(const Allocator & = Allocator())
		: m_Start(0), m_Finish(0), m_End_of_storage(0); // init memnber to 0
	{}

	// ! value에 T()를 넣으면 이 값으로 초기화 되는데, 이때 default value가 몇인가?
	// value를 n만큼 삽입.
	explicit vector(size_type n, const T &value = T(), const Allocator & = Allocator())
	{
		m_Start = allocator_type.allocate(n);

		// ! 아래 함수를 정말 사용해도 될지는 체크하기....
		std::uninitialized_fill_n(start, n, value); // using function at <memory.h> -> 값 초기화. 만약 class가 들어오면 생성자가 전부 호출되지 않을까.

		m_Finish = m_Start + n;
		m_End_of_storage = m_Finish;
	}

	// https://cplusplus.com/reference/vector/vector/vector/
	// copy constructor via other vector's iterator
	template <class InputIterator>
	vector(InputIterator first, InputIterator last, const Allocator & = Allocator())
	{
		m_Start = allocator_type.allocate(n);
		// get diff hop. C++98. calling this function will update iterator_traits?
		std::distance()


	}


	// copy constructor via other vector
	vector(const vector<T, Allocator> &x)
	{

	}

	~vector()
	{

	}

	vector<T, Allocator> &operator=(const vector<T, Allocator> &x)
	{

	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{

	}

	void assign(size_type n, const T &u)
	{

	}

	allocator_type get_allocator() const
	{

	}

	// ---------------------------------------------------------------------------
	// iterators:
	iterator begin();

	const_iterator begin() const;

	iterator end();

	const_iterator end() const;

	reverse_iterator rbegin();

	const_reverse_iterator rbegin() const;

	reverse_iterator rend();

	const_reverse_iterator rend() const;

	// 23.2.4.2 capacity:
	size_type size() const;

	size_type max_size() const;

	void resize(size_type sz, T c = T());

	size_type capacity() const;

	bool empty() const;

	void reserve(size_type n);

	// ---------------------------------------------------------------------------
	// element access:

	reference operator[](size_type n);

	const_reference operator[](size_type n) const;

	const_reference at(size_type n) const;

	reference at(size_type n);

	reference front();

	const_reference front() const;

	reference back();

	const_reference back() const;

	// ---------------------------------------------------------------------------
	// 23.2.4.3 modifiers:

	void push_back(const T &x);

	void pop_back();

	iterator insert(iterator position, const T &x);

	void insert(iterator position, size_type n, const T &x);

	template <class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);

	iterator erase(iterator position);

	iterator erase(iterator first, iterator last);

	void swap(vector<T, Allocator> &);

	void clear();

};






} // namespace ft

#endif //FT_CONTAINER_VECTOR_HPP
