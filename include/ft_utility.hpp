//
// Created by 김민규 on 2022/12/28.
//

#ifndef FT_CONTAINER_FT_UTILITY_HPP
#define FT_CONTAINER_FT_UTILITY_HPP

// --------------------------------------------------------------------------------*
//  ft_container subject 에서 명시한 직접 구현 사항들 입니다.                             *
//  template helper function을 최대한 숨기기 위해, [_PRIVATE] namespace를 이용하였습니다.  *
//  자세한 namespace definition은 "__config.hpp" 소스를 참고하시기 바랍니다.               *
// --------------------------------------------------------------------------------*

#include "__config.hpp"
#include "ft_type_traits.hpp"
FT_BEGIN_PRIVATE_NAMESPACE

// * NOTE:  m_Allocator.destroy(...) 를 사용하지 않았습니다.
// - Because destroy function is Optional, better call destructor directly.
// - Reference : https://saco-evaluator.org.za/docs/cppreference/en/cpp/concept/Allocator.html
// - 일단 custom allocator에서 destory와 construct를 구현하지 않을 수 있다는 전제하에 구현함.

template <typename Tp>
// FT_DEPRECATED // deprecated since C++17
inline void destroy(Tp* _pointer)
{
    _pointer->Tp::~Tp(); // directly call destructor.
}

template <class Iterator>
// FT_DEPRECATED // deprecated since C++17
inline void destroy(Iterator _first, Iterator _last)
{
    for (; _first != _last; ++_first) {
        destroy(&(*_first)); // pass pointer to the element.
    }
}

template <typename Tp>
// FT_DEPRECATED // deprecated since C++17
inline void construct(Tp* _pointer)
{
    // [ placement new ] : construct objects in pre-allocated storage.
    new(_pointer) Tp();
}

template <typename Tp>
// FT_DEPRECATED // deprecated since C++17
inline void construct(Tp* _pointer, const Tp& _value)
{
    // [ placement new ] : construct objects in pre-allocated storage.
    new(_pointer) Tp(_value);
}
FT_END_PRIVATE_NAMESPACE


// -------------------------------------------------------------

FT_BEGIN_GLOBAL_NAMESPACE
// * (1) std::equal() : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/equal
// 순회하면서 하나라도 틀리면 바로 false return.
template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2 )
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2))
            return false;
    }
    return true;
};


// * (2) std::equal() : [ Defined in header <algorithm> ]
// Binary predicate which returns ​true if the elements should be treated as equal.
// The signature of the predicate function should be equivalent to the following:
// - bool pred(const Type1 &a, const Type2 &b);
template< class InputIt1, class InputIt2, class BinaryPredicate >
bool equal( InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p )
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!(p(*first1, *first2)))
            return false;
    }
    return true;
}


// * (1) std::lexicographical_compare() : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
// Checks if the first range [first1, last1) is
// lexicographically less than the second range [first2, last2).
template< class InputIt1, class InputIt2 >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2 )
{
   for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
    {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return (first1 == last1) && (first2 != last2);
}

// * (2) std::lexicographical_compare() : [ Defined in header <algorithm> ]
// comparison function object (i.e. an object that satisfies the requirements of Compare)
// which returns ​true if the first argument is less than the second.
// The signature of the comparison function should be equivalent to the following:
// - bool cmp(const Type1 &a, const Type2 &b);
template< class InputIt1, class InputIt2, class Compare >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2, Compare comp )
{
    for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return (first1 == last1) && (first2 != last2);
}

// * (3) std::swap (choice) : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/swap
template <typename T>
void swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}

// * (4) std::pair : [ Defined in header <utility> ]
// https://en.cppreference.com/w/cpp/utility/pair
template <class T1, class T2>
struct pair
{
    typedef T1  first_type;
    typedef T2  second_type;

    T1  first;
    T2  second;

    pair(const T1& _t1, const T2& _t2)
        : first(_t1), second(_t2)
    {}
};

// * (5) std::make_pair : [ Defined in header <utility> ]
// https://en.cppreference.com/w/cpp/utility/pair/make_pair
template <class T1, class T2>
pair<T1, T2> make_pair( T1 t, T2 u )
{
    return (pair<T1, T2>(t, u));
}

FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER_FT_UTILITY_HPP
