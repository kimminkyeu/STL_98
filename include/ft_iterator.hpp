#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP

#include <iterator>  // for iterator_tags
#include <cstddef>
#include "__config.hpp"

FT_BEGIN_GLOBAL_NAMESPACE


/* ------------------------------------------------------------------------
 * * why do we need iterator_traits ?

    template<class Iter>
    void myAlgorithm(Iter begin, Iter end)
    {
        -----------------------------------------------------------
        | 현재 이 상황에서 알고리즘 작성자는 Iter가 어떤 컨테이너인지 모른다.   |
        | 몰라도 문제가 없도록 generic한 알고리즘을 작성하는 것이 중요하다.    |
        | 그래서 container 종류에 관계 없이 iterator의 정보를 얻어낼 수 있는 |
        | Uniform 한 데이터 접근 방법을 약속 해야한다.                    |
        | iterator_traits는 그러한 "접근 방법" 이라고 보면 된다.          |
        -----------------------------------------------------------

        typename ft::iterator_traits<Iter>::value_type temp = *begin;
        for (; begin != end; ++begin) {
            // do something ...
        }
    }

    c++11 이후에 생긴 allocator_traits도 같은 목적이다.
    그 이전에는 알고리즘에서 서로 다른 custom_allocator 를 쓰는
    컨테이너간의 알고리즘 적용은 undefined_behavior였으나
    이후엔 이것에 대한 해결법이 제안되면서, allocator_traits가 등장하였다.
    -------------------------------------------------------------------*/




// ---------------------------------------------------------------
// |                                                             |
// |              Iterator Traits implementation                 |
// |                                                             |
// ---------------------------------------------------------------
// @ Iterator tags ( used tags in namespace std ) = [ Tag dispatching ]
// struct input_iterator_tag {};
// struct output_iterator_tag {};
// struct forward_iterator_tag: public input_iterator_tag {};
// struct bidirectional_iterator_tag: public forward_iterator_tag {};
// struct random_access_iterator_tag: public bidirectional_iterator_tag {};

// * Iterator Traits.
// ----------------------------------------------------------------
template <class Iterator>
struct iterator_traits // Basic Template
{
	typedef typename Iterator::difference_type      difference_type;
	typedef typename Iterator::value_type           value_type;
	typedef typename Iterator::pointer              pointer;
	typedef typename Iterator::reference            reference;
	typedef typename Iterator::iterator_category    iterator_category;
};
// The standard library provides two partial specializations for pointer types T*,
// which makes it possible to use all iterator-based algorithms with raw pointers.

// Specializations (1) on <T*>
template<class T>
struct iterator_traits<T*> // Specializtion if T is pointer
{
	typedef std::ptrdiff_t                      difference_type;
	typedef T                                   value_type;
	typedef T*                                  pointer;
	typedef T&                                  reference;
	typedef std::random_access_iterator_tag     iterator_category;
};

// Specializations (2) on <const T*>
template<class T>
struct iterator_traits<const T*> // Specialization if T is const pointer
{
	typedef std::ptrdiff_t                      difference_type;
	typedef T                                   value_type;
	typedef const T*                            pointer;
	typedef const T&                            reference;
	typedef std::random_access_iterator_tag     iterator_category;
};



// ---------------------------------------------------------------
// |                                                             |
// |          Random Access Iterator implementation              |
// |                                                             |
// ---------------------------------------------------------------
// @ basic iterator adapter.
// This iterator adapter is 'normal' in the sense that it does not
// change the semantics of the operators of its iterator parameter.
// * Its primary purpose is to convert an iterator that is not a class,
// *  e.g. a pointer, into an iterator that is a class.
// The _Container parameter exists solely so that different containers using this template
// can instantiate different types, even if the _Iterator parameter is the same.
template<typename Iterator, typename Container>
class random_access_iterator
: public std::iterator<typename FT::iterator_traits<Iterator>::iterator_category,
                       typename FT::iterator_traits<Iterator>::value_type,
                       typename FT::iterator_traits<Iterator>::difference_type,
                       typename FT::iterator_traits<Iterator>::pointer,
                       typename FT::iterator_traits<Iterator>::reference>
{
protected:
  Iterator m_Current;

public:
    typedef typename FT::iterator_traits<Iterator>      Traits;
    typedef typename Traits::iterator_category          iterator_category;
    typedef typename Traits::value_type                 value_type;
    typedef typename Traits::difference_type            difference_type;
    typedef typename Traits::pointer                    pointer;
    typedef typename Traits::reference                  reference;

private:
    typedef random_access_iterator<Iterator, Container> random_access_iterator_type;

public: // constructor & destructor

    // NOTE:  원래 base()는 reverse_iterator에 있는 애들이다.
    // base는 wrapper class에서 원본을 찾아 제공하기 위한 수단이다.

    const Iterator& base() const
    {
        return m_Current;
    }

    // constructor
    random_access_iterator()
        : m_Current()
    {}

    // constructor Wrapping
    explicit random_access_iterator(const Iterator& _iterator_in)
        : m_Current(_iterator_in)
    {}

    // copy constructor
    template<typename Iter>
    explicit random_access_iterator(const random_access_iterator<Iter, Container>& _iterator_in)
        : m_Current(_iterator_in.base()) // wrapper가 감싸고 있는 부분을 깊은 복사하는 것.
    {}

public: // operator

     //. *normal_itr
    reference operator*() const
    { return (*m_Current); }

     //. (normal_itr->)
    pointer operator->() const
    { return m_Current; }


    random_access_iterator_type operator++(int)
    { return random_access_iterator(m_Current++); }


    random_access_iterator_type& operator++()
    { ++m_Current; return *this; }


    random_access_iterator_type operator--(int)
    { return random_access_iterator(m_Current--); }


    random_access_iterator_type& operator--()
    { --m_Current; return *this; }

public: // random access iterator requirements


    reference operator[](const difference_type& _index) const
    { return m_Current[_index]; }


    random_access_iterator_type operator+=(const difference_type& _index)
    {
        m_Current += _index;
        return *this;
    }


    random_access_iterator_type operator+(const difference_type& _index) const
    { return random_access_iterator(m_Current + _index); }


    random_access_iterator_type operator-=(const difference_type& _index)
    {
        m_Current -= _index;
        return *this;
    }

    // iter - n

    random_access_iterator_type operator-(const difference_type& _index) const
    { return random_access_iterator(m_Current - _index); }

    // iter1 - iter2
    difference_type operator-(const random_access_iterator_type& _other_iterator) const
    { return (*this).m_Current - _other_iterator.m_Current; }
};

// default random_access_iterator( aka. __normal_iterator )는 모든 기능이 가능함.
// 따라서 forward iterator의 요구사항또한 충족해야 함.
// 얘네들이 왜 class 밖에 있냐면, iterator간의 +/- 연산은 동등한 레벨 class간의 연산이기에
// 연산자가 한 클래스의 멤버로 종속될 필요가 없기 때문이다.
// @ forward iterator requirements.
// left-hand-side / right_hand_side

template<typename _IteratorL, typename _IteratorR, typename Container>
bool operator==(const random_access_iterator<_IteratorL, Container>& __lhs,
                const random_access_iterator<_IteratorR, Container>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename Container>
bool operator!=(const random_access_iterator<_IteratorL, Container>& __lhs,
                const random_access_iterator<_IteratorR, Container>& __rhs)
{ return !(__lhs == __rhs); }

// @ random access iterator requirements

// iterL < iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator<(const random_access_iterator<_IteratorL, _Container>& __lhs,
               const random_access_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() < __rhs.base(); }

// iterL > iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator>(const random_access_iterator<_IteratorL, _Container>& __lhs,
               const random_access_iterator<_IteratorR, _Container>& __rhs)
{ return __rhs < __lhs; }

// iterL <= iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator<=(const random_access_iterator<_IteratorL, _Container>& __lhs,
                const random_access_iterator<_IteratorR, _Container>& __rhs)
{ return !(__rhs < __lhs); }

// iterL >= iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator>=(const random_access_iterator<_IteratorL, _Container>& __lhs,
                const random_access_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs < __rhs); }

// iterL + iterR
template<typename _Iterator, typename _Container>
inline random_access_iterator<_Iterator, _Container>
operator+(typename random_access_iterator<_Iterator, _Container>::difference_type __n, const random_access_iterator<_Iterator, _Container>& __i)
{ return random_access_iterator<_Iterator, _Container>(__i.base() + __n); }



/*-------------------------------------------------------------------------
template<typename _IteratorL, typename _IteratorR, typename _Container>
inline typename random_access_iterator<_IteratorL, _Container>::difference_type
operator-(const random_access_iterator<_IteratorL, _Container>& __lhs,
                const random_access_iterator<_IteratorR, _Container>& __rhs)
{ return __rhs.m_Current - __lhs.m_Current; } */


// ---------------------------------------------------------------
// |                                                             |
// |              Reverse Iterator implementation                |
// |                                                             |
// ---------------------------------------------------------------
// This is the new version of reverse_iterator, as defined in the draft C++ standard.
// It relies on the iterator_traits template, which in turn relies on partial specialization.
// The class reverse_bidirectional_iterator is no longer part of the draft standard,
// but it is retained for backward compatibility.
// Ref (1) : https://en.cppreference.com/w/cpp/iterator/reverse_iterator


template <class Iterator>
class reverse_iterator
: public std::iterator<typename iterator_traits<Iterator>::iterator_category,
                       typename iterator_traits<Iterator>::value_type,
                       typename iterator_traits<Iterator>::difference_type,
                       typename iterator_traits<Iterator>::pointer,
                       typename iterator_traits<Iterator>::reference>
{
public: // typedefs
	typedef Iterator                                                iterator_type;
    typedef reverse_iterator<Iterator>                              _Self;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
	typedef typename iterator_traits<Iterator>::difference_type     difference_type;
	typedef typename iterator_traits<Iterator>::reference           reference;

protected: // data member
	Iterator m_Current;

public: // constructor & destructor

	reverse_iterator()
    {}

	explicit reverse_iterator(iterator_type _x)
        : m_Current(_x)
    {}

	template <class _Iter>
	explicit reverse_iterator(const reverse_iterator<_Iter>& _x)
        : m_Current(_x.base())
    {}

    reverse_iterator(const _Self& _x) // self copy
        : m_Current(_x.m_Current)
    {}

	template <class _Iter>
	reverse_iterator& operator=(const reverse_iterator<_Iter>& _x)
    { m_Current = _x.base(); }

	Iterator base() const
    { return m_Current; }

	reference operator*() const
    {
        iterator_type _tmp = m_Current;
        return *(--_tmp);
    }

	pointer   operator->() const
    { return &(operator*()); }

	_Self& operator++()
    {
        --m_Current;
        return *this;
    }

    _Self  operator++(int)
    {
        _Self _tmp = *this;
        --m_Current;
        return _tmp;
    }

    _Self& operator--()
    {
        ++m_Current;
        return *this;
    }

    _Self  operator--(int)
    {
        _Self _tmp = *this;
        ++m_Current;
        return _tmp;
    }

    _Self operator+(difference_type _n) const
    { return _Self(m_Current - _n); }

    _Self& operator+=(difference_type _n)
    {
        m_Current -= _n;
        return *this;
    }
    _Self operator-(difference_type _n) const
    {
        return _Self(m_Current + _n);
    }
    _Self& operator-=(difference_type _n) {
        m_Current += _n;
        return *this;
    }
    reference operator[](difference_type _n) const
    { return *(*this + _n); }
};

template <class Iterator>
bool operator==(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return x.base() == y.base(); }

template <class Iterator>
bool operator<(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return x.base() < y.base(); }

template <class Iterator>
bool operator!=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return !(x == y); }

template <class Iterator>
bool operator>(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return y < x; }

template <class Iterator>
bool operator>=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return !(x < y); }

template <class Iterator>
bool operator<=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return !(y < x); }

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{ return y.base() - x.base(); }

template <class Iterator>
reverse_iterator<Iterator> operator+(
        typename reverse_iterator<Iterator>::difference_type n,
        const reverse_iterator<Iterator>& x)
{ return reverse_iterator<Iterator>(x.base() - n); }

FT_END_GLOBAL_NAMESPACE

#endif // FT_CONTAINER_ITERATOR_HPP
