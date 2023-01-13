// --------------------------------
// Created by 김민규 on 2022/12/21.
// --------------------------------

#ifndef FT_CONTAINER_VECTOR_HPP
#define FT_CONTAINER_VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "__config.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "iterator.hpp"

FT_BEGIN_GLOBAL_NAMESPACE


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

// * -----------------------------------------------------------------------------------------------------------------
// * | libcpp vector의 경우, m_Start, m_Finish, m_End_of_storage가 쓰이는 모든 경우의 수를 Vector_base의 멤버함수로 정리하였다.    |
// * | 이를 통해 Vector class에서 위 m_멤버들을 감추고 있다. [ 멤버 데이터 캡슐화 ]                                               |
// ? | 이렇게 함으로써 얻는 이점은 무엇일까? 아직 잘 모르겠다.                                                                    |
// * | 또한 HIDE_FROM_ABI를 통해 해당 함수들의 이름을 Translation Unit에서 감추어 보안성을 높혔다.                                  |
// * -----------------------------------------------------------------------------------------------------------------
// 아래 구현 버전은 libcpp의 구현 구조를 완전히 따르지 않는다.
// 예를 들어, vector_base의 m_Finish 멤버 데이터를 vector 에서 접근한다.


template <class Tp, class Allocator>
class Vector_base
{
protected: // typedef and namespace scope
    typedef Allocator        						allocator_type;
    typedef typename allocator_type::size_type		size_type;
    typedef typename allocator_type::pointer		pointer;

protected:
    // ------------------------------
    allocator_type m_Data_allocator;
    // ------------------------------
    pointer m_Start;
    pointer m_Finish;
    pointer m_End_of_storage;

    FT_HIDE_FROM_ABI
    pointer m_Allocate(size_t _n)
    { return m_Data_allocator.allocate(_n); }

    FT_HIDE_FROM_ABI
    void m_Deallocate(pointer _p, size_t _n)
    { if (_p) { m_Data_allocator.deallocate(_p, _n); } }

    // reallocate memory + copy existing data to new memory region.
    FT_HIDE_FROM_ABI
    void m_Reallocate(size_t newCapacity)
    {
        // exception0. maxsize
        if (newCapacity > m_Data_allocator.max_size()) {
          throw std::length_error("vector");
        }

        // exception1. no size_change
        const size_type _capacity = m_End_of_storage - m_Start;
        if (newCapacity == size_t(m_End_of_storage - m_Start))
            return;

        // 1. allocate a new block of memory
        // iterator new_start = m_Allocator.allocate(newCapacity);
        pointer new_start = m_Allocate(newCapacity);

        // 2. copy/move old elements into new block
        size_type diff = 0;
        const size_type _size = m_Finish - m_Start;
        if (newCapacity < _size) {
            diff = _size - newCapacity;
        }
        pointer new_finish = std::uninitialized_copy(m_Start, m_Finish - diff, new_start);

        // 3. delete original & change m_iterator to point new block
        _PRIVATE::destroy(m_Start, m_Finish);

        m_Deallocate(m_Start, _capacity);
        m_Start = new_start;
        m_Finish = new_finish;
        m_End_of_storage = new_start + newCapacity;
    }

    FT_HIDE_FROM_ABI
    explicit Vector_base(const allocator_type& _a)
        : m_Data_allocator(_a), m_Start(0), m_Finish(0), m_End_of_storage(0)
    {}

    FT_HIDE_FROM_ABI
    explicit Vector_base(size_t _n, const allocator_type& _a)
        : m_Data_allocator(_a), m_Start(0), m_Finish(0), m_End_of_storage(0)
    {
        m_Start = m_Allocate(_n);
        m_Finish = m_Start;
        m_End_of_storage = m_Start + _n;
    }

    ~Vector_base()
    { m_Deallocate(m_Start, m_End_of_storage - m_Start); }


    // ---------------------------------------------------------
    FT_HIDE_FROM_ABI
    size_type capacity() const _NOEXCEPT
    { return m_End_of_storage - m_Start; }

    FT_HIDE_FROM_ABI
    void clear() _NOEXCEPT
    {
        _PRIVATE::destroy(m_Start, m_Finish);
        m_Finish = m_Start;
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
    typedef Vector_base<T, Allocator>                   _Vector_base;
                                                        using _Vector_base::m_Finish;
                                                        using _Vector_base::m_Start;
                                                        using _Vector_base::m_End_of_storage;
                                                        using _Vector_base::m_Data_allocator;
                                                        using _Vector_base::m_Allocate;
                                                        using _Vector_base::m_Deallocate;
                                                        using _Vector_base::m_Reallocate;

    typedef vector<T, Allocator>                        vector_type;

public:
    typedef T                                           value_type;
    typedef typename _Vector_base::allocator_type       allocator_type;
    typedef typename allocator_type::size_type          size_type;              // size_t n std::allocator
    typedef typename allocator_type::difference_type    difference_type;        // ptrdiff_t on std::allocator
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::pointer            pointer;                // _Tp* on std::allocator
    typedef typename allocator_type::const_pointer      const_pointer;          // const _Tp* on std::allocator

    typedef typename FT::random_access_iterator<pointer, vector_type>           iterator;
    typedef typename FT::random_access_iterator<const_pointer, vector_type>     const_iterator;
    typedef typename FT::reverse_iterator<iterator>			                    reverse_iterator;
    typedef typename FT::reverse_iterator<const_iterator>                       const_reverse_iterator;

public:
    // 23.2.4.1 construct/copy/destroy:
    // ---------------------------------------------------------------------------

    //* Constructs an empty container, with no elements
    FT_HIDE_FROM_ABI
    explicit vector(const allocator_type& _allocator = allocator_type())
    // _NOEXCEPT_(is_nothrow_copy_constructible<allocator_type>::value)
        : _Vector_base(_allocator)
    {}

    //* Constructs a container with n elements. Each element is a copy of val.
    FT_HIDE_FROM_ABI
    explicit vector(size_type _n, const T &_value = T(), const allocator_type& _allocator = allocator_type())
    // may throw exception
        : _Vector_base(_n, _allocator)
    {
        std::uninitialized_fill_n(m_Start, _n, _value); // using function at <memory.h>, Cpp98
        m_Finish = m_Start + _n;
    }

    // enable_if 는 여기에서 필요하다. 왜냐하면 std::vector<int>(10, 20)이 어떤 생성자인지 모르기 때문이다.
    // vector에서 iterator는 포인터이고, 주소값을 가지기에 integral 범위 내에선 위 두번째 생성자와 구분할 수 있는 기준이 없다.
    // 따라서 컴파일 에러가 발생한다.
    // https://cplusplus.com/reference/vector/vector/vector/
    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& _allocator = allocator_type(),
                typename FT::enable_if<!(FT::is_integral<InputIterator>::value)>::type * = 0)
                // if not integral, then value is false.
                // may throw exception
        : _Vector_base(std::distance(first, last), _allocator)
    {
        m_Finish = std::uninitialized_copy(first, last, m_Start);
    }

    //* Copy constructor
    explicit vector(const FT::vector<T, allocator_type> &x)  // may throw exception
        : _Vector_base(x.size(), x.get_allocator())
    {
        m_Finish = std::uninitialized_copy(x.begin(), x.end(), m_Start);
    }

    FT_HIDE_FROM_ABI
    ~vector() // may throw exception
    {
        // (1) call destructor of vector_alloc_base
        // (2) call destructor of vector_base
        _PRIVATE::destroy(m_Start, m_Finish);
    }

    FT_HIDE_FROM_ABI
    vector_type& operator=(const vector_type& other) // may throw exception
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

    // ---------------------------------------------------------------------------
    // iterators:

    FT_HIDE_FROM_ABI iterator begin() _NOEXCEPT
    { return iterator(m_Start); }

    FT_HIDE_FROM_ABI const_iterator begin() const _NOEXCEPT
    { return const_iterator(m_Start); }

    FT_HIDE_FROM_ABI iterator end() _NOEXCEPT
    { return iterator(m_Finish); }

    FT_HIDE_FROM_ABI const_iterator end() const _NOEXCEPT
    { return const_iterator(m_Finish); }

    FT_HIDE_FROM_ABI reverse_iterator rbegin() _NOEXCEPT
    { return reverse_iterator(end()); }

    FT_HIDE_FROM_ABI const_reverse_iterator rbegin() const _NOEXCEPT
    { return const_reverse_iterator(end()); }

    FT_HIDE_FROM_ABI reverse_iterator rend() _NOEXCEPT
    { return reverse_iterator(begin()); }

    FT_HIDE_FROM_ABI const_reverse_iterator rend() const _NOEXCEPT
    { return const_reverse_iterator(begin()); }


    // ---------------------------------------------------------------------------
    // 23.2.4.2 capacity:

    //* Returns the number of elements in the vector.
    FT_HIDE_FROM_ABI
    size_type size() const _NOEXCEPT
    { return static_cast<size_type>(end() - begin()); }

    //* Returns the maximum object size supported by the allocator
    //  Didn't use allocator's max_size(), because it's optinal function.
    //  REF (1) : https://en.cppreference.com/w/cpp/memory/allocator/max_size
    //  REF (2) : https://en.cppreference.com/w/cpp/named_req/Allocator --> optional.
    size_type max_size() const _NOEXCEPT
    { return (std::numeric_limits<size_type>::max() / sizeof(value_type)); }

    //* Returns the size of the storage space currently allocated for the vector, expressed in terms of elements.
    FT_HIDE_FROM_ABI
    size_type capacity() const _NOEXCEPT
    { return _Vector_base::capacity(); }

    FT_HIDE_FROM_ABI
    bool empty() const _NOEXCEPT
    { return ( begin() == end()); }

    //* Resizes the container so that it contains n elements.
    void resize(size_type _n, T _value = T()) // may throw exception
    {
        if (_n == this->size())
            return ;
        else if (_n < this->size())
            _PRIVATE::destroy(m_Start + _n, m_Finish);
        else // n > this->size()
        {
            if (_n > this->capacity()) // if n is also greater than capacity
                m_Reallocate((capacity() * 2) + _n);
            std::uninitialized_fill_n(m_Finish, _n - size(), _value);
        }
        m_Finish = m_Start + _n;
    }

    //* Requests that the vector capacity be at least enough to contain n elements.
    void reserve(size_type n) // may throw exception
    {
        if (n > this->capacity())
            m_Reallocate(n);
    }

    // ---------------------------------------------------------------------------
    // element access:

    FT_HIDE_FROM_ABI allocator_type get_allocator() const _NOEXCEPT
    { return m_Data_allocator; }

    FT_HIDE_FROM_ABI value_type* data() _NOEXCEPT
    { return m_Start; }

    FT_HIDE_FROM_ABI const value_type* data() const _NOEXCEPT
    { return m_Start; }

    FT_HIDE_FROM_ABI reference operator[](size_type n) _NOEXCEPT
    { return *(begin() + n); }

    FT_HIDE_FROM_ABI const_reference operator[](size_type n) const _NOEXCEPT
    { return *(begin() + n); }

    FT_HIDE_FROM_ABI reference front() _NOEXCEPT
    { return *begin(); }

    FT_HIDE_FROM_ABI const_reference front() const _NOEXCEPT
    { return *begin(); }

    FT_HIDE_FROM_ABI reference back() _NOEXCEPT
    { return *(--end()); }

    FT_HIDE_FROM_ABI const_reference back() const _NOEXCEPT
    { return *(--end()); }

    const_reference at(size_type n) const // may throw exception
    {
       if (n > this->size()) {
            throw std::out_of_range("vector");
        }
        return this->operator[](n);
    }

    reference at(size_type n)  // may throw exception
    {
        if (n > this->size()) {
            throw std::out_of_range("vector");
        }
        return this->operator[](n);
    }

    // ---------------------------------------------------------------------------
    // 23.2.4.3 modifiers:

    //* Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
    // assign도 integral 형에 대한 enable_if가 필요하다.
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename FT::enable_if<!(FT::is_integral<InputIterator>::value)>::type * = 0)
                // may throw exception
    {
        this->clear();
        m_Reallocate(last - first);
        m_Finish = std::uninitialized_copy(first, last, m_Start);
    }

    void assign(size_type n, const T &value) // may throw exception
    {
        // 벡터 객체에 이전에 있었던 원소들은 모두 삭제하고, 인자로 받은 새로운 내용을 집어 넣는다. 원래 내용을 다 지우고 원소 u 를 n 개 가지는 벡터로 만든다.
        this->clear();
        if (n > this->capacity())
            m_Reallocate(n);
        std::uninitialized_fill_n(m_Start, n, value);
        m_Finish = m_Start + n;
    }

    FT_HIDE_FROM_ABI
    void clear() _NOEXCEPT
    {
        _Vector_base::clear();
    }

    FT_HIDE_FROM_ABI
    void push_back(const T &value) // may throw exception
    {
        if (this->capacity() == 0) {
            reserve(1);
        }
        else if (this->size() >= this->capacity()) {
            m_Reallocate(this->capacity() * 2);
        }
        _PRIVATE::construct(m_Finish, value);
        ++m_Finish;
    }

    FT_HIDE_FROM_ABI
    void pop_back() _NOEXCEPT
    {
        if (this->empty()) {
            return ;
        }
        _PRIVATE::destroy(--m_Finish);
    }

    iterator insert(iterator _position, const T& _value) // may throw exception
    {
        const difference_type insert_pos = std::distance(begin(), _position);
        if (_position == end()) // if insert at the end.
        {
            push_back(_value);
            return (iterator(&((*this)[insert_pos])));
        }
        else // if insert in the middle
        {
            resize(size() + 1); // ++m_Finish
            // 메모리 재할당이 일어날 경우 기존 iterator는 해제된 메모리를 가리키므로, 반드시 업데이트해야 한다.
            const iterator safe_iter = iterator(&((*this)[insert_pos]));
            std::copy_backward(safe_iter, end() - 1, end());
            *safe_iter = _value;
            return (safe_iter);
        }
    }

    void insert(iterator _position, size_type n, const T &value) // may throw exception
    {
        const difference_type insert_pos = std::distance(begin(), _position);

        // (1) 뒷 부분 따로 보유.
        FT::vector<T> tmp(_position, end());
        _PRIVATE::destroy(_position, end());

        // (2) 공간 필요시 확장.
        if (this->size() + n >= this->capacity()) {
            m_Reallocate((this->capacity() * 2) + n); // may throw exception
        }
        // (3) position 부터 value n개 삽입.
        // 이때 메모리 재할당이 일어날 경우 기존 iterator는 해제된 메모리를 가리키므로, 반드시 업데이트해야 한다.
        const iterator safe_iter = iterator(&((*this)[insert_pos]));
        std::uninitialized_fill_n(safe_iter, n, value);
        // (4) position + n 부터 백업본 복사.
        const iterator _backup_finish = std::uninitialized_copy(tmp.begin(), tmp.end(), safe_iter + n);
        m_Finish = _backup_finish.base();
    }

    template <class InputIterator>
    void insert(iterator _position, InputIterator first, InputIterator last,
                typename FT::enable_if<!(FT::is_integral<InputIterator>::value)>::type * = 0)
                // may throw exception
    {
        const difference_type insert_pos = std::distance(begin(), _position);

        // (0) first 부터 last 까지 개수 구하기
        const difference_type sizeToCopy = std::distance(first, last);

        // (1) 뒷 부분 따로 보유.
        FT::vector<T> tmp(_position, end());
        _PRIVATE::destroy(_position, end());

        // (2) 공간 필요시 확장.
        if (this->size() + sizeToCopy >= this->capacity()) {
            m_Reallocate((this->capacity() * 2) + sizeToCopy);
        }

        // 이때 메모리 재할당이 일어날 경우 기존 iterator는 해제된 메모리를 가리키므로, 반드시 업데이트해야 한다.
        const iterator safe_iter = iterator(&((*this)[insert_pos]));

        // (3) position 부터 value n개 삽입.
        const iterator _insert_finish = std::uninitialized_copy(first, last, safe_iter);
        m_Finish = _insert_finish.base();

        // (4) position + sizeToCopy 부터 백업본 복사.
        const iterator _backup_finish = std::uninitialized_copy(tmp.begin(), tmp.end(), safe_iter + sizeToCopy);
        m_Finish = _backup_finish.base();
    }

    FT_HIDE_FROM_ABI
    iterator erase(iterator position) // may throw exception
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
        while (cur != iterator(m_Finish - 1)) {
            FT::swap(*cur, *(++cur));
        }
        pop_back();
        return (position);
    }

    // last - 1 까지 지움.
    FT_HIDE_FROM_ABI
    iterator erase(iterator first, iterator last) // may throw exception
    {
        // b    f      l
        // 1 2 (3) (4) 5 6

        // b
        // 1 2              5 6

        // b
        // 1 2 5 6  --> return 5*

        const difference_type tmp_len = std::distance(begin(), first);

        // (1) 삭제하는 뒷 부분 따로 보유. (5, 6)
        FT::vector<T> tmp(last, end());

        // (2) first 이후 부터 싹 다 제거.
        _PRIVATE::destroy(first, end());

        // (3) first 로 백업본 복사.
        m_Finish = std::uninitialized_copy(tmp.begin(), tmp.end(), m_Start + tmp_len);

        return begin() + tmp_len;
    }


    void swap(FT::vector<T, allocator_type>& other)
    // * C++17 이전까지는 예외를 던지지 않는다.
    /* C++ 17 이후에 추가된 Exception
                    _NOEXCEPT_(!__alloc_traits::propagate_on_container_swap::value ||
                     __is_nothrow_swappable<allocator_type>::value); */
    {
        // 두 벡터간 데이터 교체가 iterator만 교체해주면 되기 때문에 몹시 쉬움.
        FT::swap(m_Start, other.m_Start);
        FT::swap(m_Finish, other.m_Finish);
        FT::swap(m_End_of_storage, other.m_End_of_storage);
    }

};

// Non-member function.
// -----------------------------------------------------------

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator==(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return x.size() == y.size() && FT::equal(x.begin(), x.end(), y.begin());
}

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator!=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return !(x == y);
}

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator< (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return FT::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator> (const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return y < x;
}

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator>=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return !(x < y);
}

template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
bool operator<=(const FT::vector<T,Allocator>& x, const FT::vector<T,Allocator>& y)
{
    return !(y < x);
}

// specialized algorithms:
template <class T, class Allocator>
inline FT_HIDE_FROM_ABI
void swap(FT::vector<T,Allocator>& x, FT::vector<T,Allocator>& y) /* (C++17) _NOEXCEPT_(_NOEXCEPT_(x.swap(y))) */
// C++17 이전에는 x.swap이 예외를 던지지 않는다.
// 그러나 C++17 이후부터는 x.swap이 예외를 던질 수 있기에 noexcept(noexcept(lhs.swap(rhs)))를 사용한다.
{
    x.swap(y);
}

FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER_VECTOR_HPP
