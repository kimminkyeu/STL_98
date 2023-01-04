#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP

// #include <cstddef>
#include <iterator>  // for iterator_tags
#include <cstddef>
#include "__config.hpp"


FT_BEGIN_GLOBAL_NAMESPACE

// @ Iterator tags ( used tags in namespace std )
// ----------------------------------------------------------------
/* [ Tag dispatching ]
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
*/

// * Iterator Traits.
// ----------------------------------------------------------------
// @ iterator_traits (1)
template <class Iterator>
struct iterator_traits
{
	typedef typename Iterator::difference_type      difference_type;
	typedef typename Iterator::value_type           value_type;
	typedef typename Iterator::pointer              pointer;
	typedef typename Iterator::reference            reference;
	typedef typename Iterator::iterator_category    iterator_category;
};

// @ iterator_traits (2) -> pointer type is always random-access-iterator
template<class T>
struct iterator_traits<T*>
{
	typedef ptrdiff_t                           difference_type;
	typedef T                                   value_type;
	typedef T*                                  pointer;
	typedef T&                                  reference;
	typedef std::random_access_iterator_tag     iterator_category;
};

// * From ISO/IEC 14882:1998(E) Header <iterator> synopsis
// * Iterator definition
// ----------------------------------------------------------
// @ basic iterator adapter.
// This iterator adapter is 'normal' in the sense that it does not
// change the semantics of any of the operators of its itererator parameter.

// * Its primary purpose is to convert an iterator that is not a class,
// *  e.g. a pointer, into an iterator that is a class.

// The _Container parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// _Iterator parameter is the same. --> ! [ libcpp에 나와있는 설명글. ]

// TODO:  ? why 이런 방식으로 ? 이 구조가 궁금하다.
/*
template<class _Category, class _Tp, class _Distance = ptrdiff_t,
         class _Pointer = _Tp*, class _Reference = _Tp&>
struct _LIBCPP_TEMPLATE_VIS iterator
{
    typedef _Tp        value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
    typedef _Category  iterator_category;
};
* 상속받는 std::iterator는 간단한 struct로, 최소한의 구조만을 가지고 있다. */

template<typename _Iterator, typename _Container>
class __normal_iterator
: public std::iterator<typename iterator_traits<_Iterator>::iterator_category,
                       typename iterator_traits<_Iterator>::value_type,
                       typename iterator_traits<_Iterator>::difference_type,
                       typename iterator_traits<_Iterator>::pointer,
                       typename iterator_traits<_Iterator>::reference>
{
protected:
  _Iterator m_Current;
  // * __normal_iterator는 그냥 _Iterator Wrapper이다.
  // * std::iterator에서 구현된 함수를 상속받기 때문에 추가로 구현할 필요가 없다.

public: //
    typedef __normal_iterator<_Iterator, _Container> normal_iterator_type;

public: // constructor & destructor

    const _Iterator& base() const
    { return m_Current; }

    // constructor
    FT_INLINE_VISIBILITY
    __normal_iterator()
        : m_Current()
    {}

    // constructor Wrapping
    FT_INLINE_VISIBILITY
    explicit __normal_iterator(const _Iterator& _iterator_in)
        : m_Current(_iterator_in)
    {}

    // copy constructor
    template<typename _Iter>
    FT_INLINE_VISIBILITY
    __normal_iterator(const __normal_iterator<_Iter, _Container>& _iterator_in)
        : m_Current(_iterator_in.base()) // wrapper가 감싸고 있는 부분을 깊은 복사하는 것.
    {}

    FT_INLINE_VISIBILITY
    reference operator*() const
    {
        return (*m_Current);
    }
//..












};





// ----------------------------------------------------------
// @ reverse_iterator

template <class Iterator>
class reverse_iterator
: public iterator<typename iterator_traits<Iterator>::iterator_category,
                       typename iterator_traits<Iterator>::value_type,
                       typename iterator_traits<Iterator>::difference_type,
                       typename iterator_traits<Iterator>::pointer,
                       typename iterator_traits<Iterator>::reference>
{
public: // typedefs
	typedef Iterator                                            iterator_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::reference       reference;
	typedef typename iterator_traits<Iterator>::pointer         pointer;

protected: // data member
	Iterator current;

public: // constructor & destructor

	reverse_iterator()
    {}

	explicit reverse_iterator(Iterator x)
    {}

	template <class U>
	reverse_iterator(const reverse_iterator<U>& u)
    {}

	template <class U>
	reverse_iterator& operator=(const reverse_iterator<U>& u)
    {}

	Iterator base() const
    {}

	reference operator*() const
    {}

	pointer   operator->() const
    {}

	reverse_iterator& operator++()
    {}

	reverse_iterator  operator++(int)
    {}

	reverse_iterator& operator--()
    {}

	reverse_iterator  operator--(int)
    {}

	reverse_iterator  operator+ (difference_type n) const
    {}

	reverse_iterator& operator+=(difference_type n)
    {}

	reverse_iterator  operator- (difference_type n) const
    {}

	reverse_iterator& operator-=(difference_type n)
    {}

	reference         operator[](difference_type n) const
    {}
};

template <class Iterator>
bool operator==(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
bool operator<(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
bool operator!=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
bool operator>(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
bool operator>=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
bool operator<=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y)
{}

template <class Iterator>
reverse_iterator<Iterator> operator+(
        typename reverse_iterator<Iterator>::difference_type n,
        const reverse_iterator<Iterator>& x)
{}






















FT_END_GLOBAL_NAMESPACE
#endif // FT_CONTAINER_ITERATOR_HPP
