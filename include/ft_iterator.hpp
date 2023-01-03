#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP


#include "__config.hpp"
FT_BEGIN_GLOBAL_NAMESPACE

#include <iterator> // delete later

#include <stddef.h> // for ptrdiff_t

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

// @ Iterator tags
// ! 이걸 직접 정의하면 std의 iterator_tag랑 호환이 안되지 않나?
/*
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
template<class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
struct iterator
// used struct because we don't need private members.
{
public:
    // 24.3, primitives:
	typedef T         value_type;
	typedef Distance  difference_type;
	typedef Pointer   pointer;
	typedef Reference reference;
	typedef Category  iterator_category;

public:
    // 24.3.4, iterator operations:
    template <class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n);

    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last);


// 24.4, predefined iterators:
    template <class Iterator> class reverse_iterator;

    template <class Iterator>
    bool operator==(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    bool operator<(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    bool operator!=(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    bool operator>(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    bool operator>=(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    bool operator<=(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type operator-(
            const reverse_iterator<Iterator>& x,
            const reverse_iterator<Iterator>& y);

    template <class Iterator>
    reverse_iterator<Iterator>
    operator+(
            typename reverse_iterator<Iterator>::difference_type n,
            const reverse_iterator<Iterator>& x);

    template <class Container> class back_insert_iterator;

    template <class Container>
    back_insert_iterator<Container> back_inserter(Container& x);
    template <class Container> class front_insert_iterator;

    template <class Container>
    front_insert_iterator<Container> front_inserter(Container& x);
    template <class Container> class insert_iterator;

    template <class Container, class Iterator>
    insert_iterator<Container> inserter(Container& x, Iterator i);

    // 24.5, stream iterators:
    template <class T, class charT = char, class traits = char_traits<charT>,
            class Distance = ptrdiff_t>
    class istream_iterator;

    template <class T, class charT, class traits, class Distance>
    bool operator==(const istream_iterator<T,charT,traits,Distance>& x,
                    const istream_iterator<T,charT,traits,Distance>& y);

    template <class T, class charT, class traits, class Distance>
    bool operator!=(const istream_iterator<T,charT,traits,Distance>& x,
                    const istream_iterator<T,charT,traits,Distance>& y);

    template <class T, class charT = char, class traits = char_traits<charT> >
    class ostream_iterator;

    template<class charT, class traits = char_traits<charT> >
    class istreambuf_iterator;

    template <class charT, class traits>
    bool operator==(const istreambuf_iterator<charT,traits>& a,
                    const istreambuf_iterator<charT,traits>& b);

    template <class charT, class traits>
    bool operator!=(const istreambuf_iterator<charT,traits>& a,
                    const istreambuf_iterator<charT,traits>& b);

    template <class charT, class traits = char_traits<charT> >
    class ostreambuf_iterator;
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

	reverse_iterator();
	explicit reverse_iterator(Iterator x);

	template <class U>
	reverse_iterator(const reverse_iterator<U>& u);

	template <class U>
	reverse_iterator& operator=(const reverse_iterator<U>& u);

	Iterator base() const;
	reference operator*() const;
	pointer   operator->() const;
	reverse_iterator& operator++();
	reverse_iterator  operator++(int);
	reverse_iterator& operator--();
	reverse_iterator  operator--(int);
	reverse_iterator  operator+ (difference_type n) const;
	reverse_iterator& operator+=(difference_type n);
	reverse_iterator  operator- (difference_type n) const;
	reverse_iterator& operator-=(difference_type n);
	reference         operator[](difference_type n) const;
};

template <class Iterator>
bool operator==(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator<(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator!=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator>(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator>=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator<=(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
        const reverse_iterator<Iterator>& x,
        const reverse_iterator<Iterator>& y);

template <class Iterator>
reverse_iterator<Iterator> operator+(
        typename reverse_iterator<Iterator>::difference_type n,
        const reverse_iterator<Iterator>& x);






















FT_END_GLOBAL_NAMESPACE
#endif // FT_CONTAINER_ITERATOR_HPP