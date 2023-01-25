//
// Created by 김민규 on 2022/12/28.
//

#ifndef FT_CONTAINER_UTILITY_HPP
#define FT_CONTAINER_UTILITY_HPP

// --------------------------------------------------------------------------------*
//  ft_container subject 에서 명시한 직접 구현 사항들 입니다.                             *
//  template helper function을 최대한 숨기기 위해, [_PRIVATE] namespace를 이용하였습니다.  *
//  자세한 namespace definition은 "__config.hpp" 소스를 참고하시기 바랍니다.               *
// --------------------------------------------------------------------------------*

#include "__config.hpp"
#include "type_traits.hpp"

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
}


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
template <class _T1, class _T2>
struct FT_TEMPLATE_VIS pair
{
    typedef _T1 first_type;
    typedef _T2 second_type;

    _T1 first;
    _T2 second;

    FT_HIDE_FROM_ABI
    pair()
        : first(), second()
    {}

    FT_HIDE_FROM_ABI
    pair(_T1 const& __t1, _T2 const& __t2)
        : first(__t1), second(__t2)
    {}

    template <class _U1, class _U2>
    FT_HIDE_FROM_ABI
    pair(const pair<_U1, _U2>& __p)
        : first(__p.first), second(__p.second)
    {}

    FT_HIDE_FROM_ABI
    pair& operator=(pair const& __p)
    {
        first = __p.first;
        second = __p.second;
        return *this;
    }

    template<class U1, class U2>
    FT_HIDE_FROM_ABI
    pair& operator=(const pair<U1, U2>& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }
};

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator==(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return __x.first == __y.first && __x.second == __y.second;
}

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator!=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return !(__x == __y);
}

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator< (const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator> (const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return __y < __x;
}

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator>=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return !(__x < __y);
}

template <class _T1, class _T2>
inline FT_HIDE_FROM_ABI
bool
operator<=(const pair<_T1,_T2>& __x, const pair<_T1,_T2>& __y)
{
    return !(__y < __x);
}


// * map의 경우 pair<const K, V>를 사용하기 때문에,
// * K를 복사하지 못한다. 따라서 이 경우에 한해 복사를 어떻게 할지 알려줘야 한다.
// * 그래서 아래와 같이 map에 대해 특수 경우를 따로 작성하였다.
/*
template <class T1, class T2>
struct pair<const T1, T2>
{
    typedef pair<const T1, T2>  pair_type;
    typedef T1  first_type;
    typedef T2  second_type;

    const T1  first;
    T2  second;

    pair(const T1& _t1, const T2& _t2)
        : first(_t1), second(_t2)
    {}

    template <class U1, class U2>
    explicit pair(const pair<U1, U2>& other_pair)
        : first(other_pair.first), second(other_pair.second)
    {}

    pair_type& operator=(const pair_type& other)
    {
        this->second = other.second; // second 만 복사 진행.
        return (*this);
    }
};
 */

// * (5) std::make_pair : [ Defined in header <utility> ]
// https://en.cppreference.com/w/cpp/utility/pair/make_pair
template <class T1, class T2>
inline FT_HIDE_FROM_ABI
pair<T1, T2> make_pair( T1 t, T2 u )
{
    return (pair<T1, T2>(t, u));
}

FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER_UTILITY_HPP
