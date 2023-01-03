#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP


#include "__config.hpp"
FT_BEGIN_GLOBAL_NAMESPACE

#include <iterator> // delete later
#include <cstddef> // for ptrdiff_t

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

// @ Iterator tags ( used tags in namespace std )
// ----------------------------------------------------------------
/* [ Tag dispatching ]
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
*/





// * From ISO/IEC 14882:1998(E) Header <iterator> synopsis
// * Iterator definition
// ----------------------------------------------------------
// @ basic iterator




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