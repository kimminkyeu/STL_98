//
// Created by 김민규 on 2022/12/28.
//

#ifndef FT_CONTAINER_FT_UTILITY_HPP
#define FT_CONTAINER_FT_UTILITY_HPP

// --------------------------------------------------------------------------------
//  ft_container subject 에서 명시한 직접 구현 사항들 입니다.
//  * template helper function을 최대한 숨기기 위해, [_PRIVATE] namespace를 이용하였습니다.
//  자세한 namespace definition은 "__config.hpp" 소스를 참고하시기 바랍니다.
// --------------------------------------------------------------------------------

#include "__config.hpp"
FT_BEGIN_PRIVATE_NAMESPACE


// * m_Allocator.destroy(start);
// --> because destroy function is optional, better call destructor directly.
// 일단 custom allocator에서 destory와 construct를 구현하지 않을 수 있다는 전제하에 구현함.
template <typename Tp>
FT_DEPRECATED // since C++17
inline void destroy(Tp* _pointer)
{
    _pointer->Tp::~Tp(); // directly call destructor.
}

template <class Iterator>
FT_DEPRECATED // since C++17
inline void destroy(Iterator _first, Iterator _last)
{
    for (; _first != _last; ++_first) {
        destroy(&(*_first)); // pass pointer to the element.
    }
}

template <typename Tp>
FT_DEPRECATED // since C++17
inline void construct(Tp* _pointer)
{
    // * [ placement new ] : construct objects in allocated storage.
    new(_pointer) Tp();
}

template <typename Tp>
FT_DEPRECATED // since C++17
inline void construct(Tp* _pointer, const Tp& _value)
{
    // * [ placement new ] : construct objects in allocated storage.
    new(_pointer) Tp(_value);
}


// (1) std::equal() : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/equal
template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2 )
{

};

// (2) std::lexicographical_compare() : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
template< class InputIt1, class InputIt2 >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2 )
{

}


// (3) std::swap (choice) : [ Defined in header <algorithm> ]
// https://en.cppreference.com/w/cpp/algorithm/swap
template <typename T>
void swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}


// (4) std::pair : [ Defined in header <utility> ]
// https://en.cppreference.com/w/cpp/utility/pair
template <class T1, class T2>
struct pair
{

};

// (5) std::make_pair : [ Defined in header <utility> ]
// https://en.cppreference.com/w/cpp/utility/pair/make_pair
template <class T1, class T2>
ft::pair<T1, T2> make_pair( T1 t, T2 u )
{

}

// (6) std::enable_if : [ Defined in header <type_traits> ]
template <bool, typename T = void>
struct enable_if
{

};

// template specialization if bool
template <typename T>
struct enable_if<true, T>
{
	typedef T type;
};

// (7) std::is_integral : [ Defined in header <type_traits> ]
template <class T>
struct is_integral
{

};


FT_END_PRIVATE_NAMESPACE
#endif //FT_CONTAINER_FT_UTILITY_HPP