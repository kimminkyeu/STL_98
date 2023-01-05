#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP

// #include <cstddef>
#include <iterator>  // for iterator_tags
#include <cstddef>
#include "__config.hpp"


FT_BEGIN_GLOBAL_NAMESPACE


// ---------------------------------------------------------------
// |                                                             |
// |              Iterator Traits implementation                 |
// |                                                             |
// ---------------------------------------------------------------

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
struct iterator_traits // Basic Template
{
	typedef typename Iterator::difference_type      difference_type;
	typedef typename Iterator::value_type           value_type;
	typedef typename Iterator::pointer              pointer;
	typedef typename Iterator::reference            reference;
	typedef typename Iterator::iterator_category    iterator_category;
};

// @ iterator_traits (2) -> pointer type is always random-access-iterator
template<class T>
struct iterator_traits<T*> // Specializtion if T is pointer
{
	typedef std::ptrdiff_t                           difference_type;
	typedef T                                   value_type;
	typedef T*                                  pointer;
	typedef T&                                  reference;
	typedef std::random_access_iterator_tag     iterator_category;
};

// @ iterator_traits (3) -> const pointer type is always random-access-iterator

// Specializations
// This type trait may be specialized for user-provided types that may be used as iterators.
// The standard library provides two partial specializations for pointer types T*,
// which makes it possible to use all iterator-based algorithms with raw pointers.

// ! [ 미해결 질문 사항 ]
// *--------------------------------------------------------------------*
// *  NOTE:  왜 굳이 T* 포인터 타입을 const랑 non-const로 나누었는가 ?           *
// *--------------------------------------------------------------------*
// *  알고리즘 구현자는 iterator가 pointer 타입인지 아닌지 구분할 필요가 없다.       *
// *  따라서 const, non-const pointer 타입 모두 일반 iterator라고 생각하고      *
// *  처리하기 위함이다.                                                    *
// *                                                                    *
// *  Ex1. iterator_traits <const int *>::reference -> [const int *]    *
// *  Ex2. iterator_traits <int *>::pointer         -> [int *]          *
// *--------------------------------------------------------------------*
// Ref : https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/iterator/iterator_traits.html

template<class T>
struct iterator_traits<const T*> // Specializtion if T is const pointer
{
	typedef std::ptrdiff_t                           difference_type;
	typedef T                                   value_type;
	typedef const T*                            pointer;
	typedef const T&                            reference;
	typedef std::random_access_iterator_tag     iterator_category;
};


struct some
{
    FT::iterator_traits<const int*>::value_type n; // --> T 가 int로 잡힌다.
};



// Iter_in -> int *

// Iter_in -> const int *

// iter = iterator_traits<Iter_in>::value_type n = 10;





// ---------------------------------------------------------------
// |                                                             |
// |          Random Access Iterator implementation              |
// |                                                             |
// ---------------------------------------------------------------
// * std::iterator에는 typedef밖에 없음. 이 구조를 받아서 전부 구현해줘야 한다.
// @ basic iterator adapter.
// This iterator adapter is 'normal' in the sense that it does not
// change the semantics of any of the operators of its itererator parameter.
// * Its primary purpose is to convert an iterator that is not a class,
// *  e.g. a pointer, into an iterator that is a class.
// The _Container parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// _Iterator parameter is the same. --> ! [ libcpp에 나와있는 설명글. ]
// * 상속받는 std::iterator는 간단한 struct로, 최소한의 구조만을 가지고 있다. */
// * 그니까 구조가, _Iterator 내부에 있는 정보들이 여기에 연결되서 들어오게 되어있음.
// 즉 _Iterator에 이미 있어야 함.
// 왜 _Container를 두번째 파라미터로 받나면, 같은 벡터여도 Allocator가 다르면 다른 애라고 구분하기 위함임.
// vector에서 __normal_iterator를 쓸 때  typedef vector<_Tp, _Alloc> vector_type; 를 넣어줌.
// 즉 같은 벡터여도 Allocator가 다르면 다른 벡터 취급하는 거임.
// * 그럼 왜 이렇게 하냐? 어따 쓸라고? 이건 라이브러리를 더 까볼 것.
// * Wrapping을 해주면, pointer type iterator에 대해서 특수화를 따로 하지 않아도 되지 않을까...?

template<typename _Iterator, typename _Container>
class __normal_iterator
: public std::iterator<typename FT::iterator_traits<_Iterator>::iterator_category,
                       typename FT::iterator_traits<_Iterator>::value_type,
                       typename FT::iterator_traits<_Iterator>::difference_type,
                       typename FT::iterator_traits<_Iterator>::pointer,
                       typename FT::iterator_traits<_Iterator>::reference>
{
protected:
  _Iterator m_Current;
  // * __normal_iterator는 그냥 _Iterator Wrapper이다.
  // * std::iterator에서 구현된 함수를 상속받기 때문에 추가로 구현할 필요가 없다.

public: // @ Using directives
        // Ref_(1) : https://stackoverflow.com/questions/51883423/c-using-and-using-typename-in-a-header-file
        // Ref_(2) : https://en.cppreference.com/w/cpp/language/namespace#Using-directives
        // "using directive" is for using typenames defined in inner namespace.
    using typename FT::iterator_traits<_Iterator>::iterator_category;
    using typename FT::iterator_traits<_Iterator>::value_type;
    using typename FT::iterator_traits<_Iterator>::difference_type;
    using typename FT::iterator_traits<_Iterator>::pointer;
    using typename FT::iterator_traits<_Iterator>::reference;

public: // typedefs
    typedef __normal_iterator<_Iterator, _Container> normal_iterator_type;

public: // constructor & destructor

    // NOTE:  원래 base()는 reverse_iterator에 있는 애들이다.
    // base는 wrapper class에서 원본을 찾아 제공하기 위한 수단이다.
    FT_INLINE_VISIBILITY
    const _Iterator& base() const FT_NOEXCEPT
    {
        return m_Current;
    }

    // constructor
    FT_INLINE_VISIBILITY
    __normal_iterator() FT_NOEXCEPT
        : m_Current()
    {}

    // constructor Wrapping
    FT_INLINE_VISIBILITY
    explicit __normal_iterator(const _Iterator& _iterator_in) FT_NOEXCEPT
        : m_Current(_iterator_in)
    {}

    // copy constructor
    template<typename _Iter>
    FT_INLINE_VISIBILITY
    __normal_iterator(const __normal_iterator<_Iter, _Container>& _iterator_in) FT_NOEXCEPT
        : m_Current(_iterator_in.base()) // wrapper가 감싸고 있는 부분을 깊은 복사하는 것.
    {}

public: // operator

    FT_INLINE_VISIBILITY //. *normal_itr
    reference operator*() const FT_NOEXCEPT
    { return (*m_Current); }

    FT_INLINE_VISIBILITY //. (normal_itr->)
    pointer operator->() const FT_NOEXCEPT
    { return m_Current; }

    FT_INLINE_VISIBILITY
    normal_iterator_type& operator++(int) FT_NOEXCEPT
    { return normal_iterator_type(m_Current++); }

    FT_INLINE_VISIBILITY
    normal_iterator_type& operator++() FT_NOEXCEPT
    { ++m_Current; return *this; }

    FT_INLINE_VISIBILITY
    normal_iterator_type& operator--(int) FT_NOEXCEPT
    { return __normal_iterator(m_Current--); }

    FT_INLINE_VISIBILITY
    normal_iterator_type& operator--() FT_NOEXCEPT
    { --m_Current; return *this; }

public: // random access iterator requirements

    FT_INLINE_VISIBILITY
    reference operator[](const difference_type& _index) const FT_NOEXCEPT
    { return m_Current[_index]; }

    FT_INLINE_VISIBILITY
    normal_iterator_type operator+=(const difference_type& _index) FT_NOEXCEPT
    {
        m_Current += _index;
        return *this;
    }

    FT_INLINE_VISIBILITY
    normal_iterator_type operator+(const difference_type& _index) const FT_NOEXCEPT
    { return __normal_iterator(m_Current + _index); }

    FT_INLINE_VISIBILITY
    normal_iterator_type operator-=(const difference_type& _index) FT_NOEXCEPT
    {
        m_Current -= _index;
        return *this;
    }

    // iter - n
    FT_INLINE_VISIBILITY
    normal_iterator_type operator-(const difference_type& _index) const FT_NOEXCEPT
    { return __normal_iterator(m_Current - _index); }

    // iter1 - iter2
    FT_INLINE_VISIBILITY
    difference_type operator-(const normal_iterator_type& _other_iterator) const FT_NOEXCEPT
    { return (*this).m_Current - _other_iterator.m_Current; }
};
// * default random_access_iterator( aka. __normal_iterator )는 모든 기능이 가능함.
// 따라서 forward iterator의 요구사항또한 충족해야 함.
// 얘네들이 왜 class 밖에 있냐면, iterator간의 +/- 연산은 동등한 레벨 class간의 연산이기에
// 연산자가 한 클래스의 멤버로 종속될 필요가 없기 때문이다.

// @ forward iterator requirements.
// * left-hand-side / right_hand_side
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
                const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
                const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs == __rhs); }

// @ random access iterator requirements

// iterL < iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
               const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() < __rhs.base(); }

// iterL > iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
               const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __rhs < __lhs; }

// iterL <= iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
                const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__rhs < __lhs); }

// iterL >= iterR
template<typename _IteratorL, typename _IteratorR, typename _Container>
bool operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
                const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs < __rhs); }

// iterL + iterR
template<typename _Iterator, typename _Container>
inline __normal_iterator<_Iterator, _Container>
operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n, const __normal_iterator<_Iterator, _Container>& __i)
{ return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }




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
