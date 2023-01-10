#ifndef FT_CONTAINER_TYPE_TRAITS_HPP
#define FT_CONTAINER_TYPE_TRAITS_HPP

#include "__config.hpp"

FT_BEGIN_GLOBAL_NAMESPACE


// -------------------------------------------------------
// |                                                     |
// |        Template 특수화를 이용해서 부리는 꼼수              |
// |                                                     |
// -------------------------------------------------------

// * (0) std::enable_if : [ Defined in header <type_traits> ]
// https://modoocode.com/295
template <bool, typename T = void>
struct enable_if {}; // if empty struct, member type doesn't exist.

// template specialization if bool
template <typename T>
struct enable_if<true, T> // only if true, then member type exists.
{
	typedef T type;
};

// * ---------------------------------------------------------------
// * [ How to use ]                                                |
// * ---------------------------------------------------------------
// * : enable_if< is_integral<bool>::value > ::type                |
// *              ------------------------                         |
// *                   true or false                               |
// *                                                               |
// *! : (1) enable_if< true >::type   --> type doesn't exist. (X)  |
// *? : (2) enable_if< flase >::type  --> type exists. (O)         |
// * ---------------------------------------------------------------


// * (1) std::integral_constant : [ Defined in header <type_traits> ]
// https://cplusplus.com/reference/type_traits/integral_constant/
// This template is designed to provide compile-time constants as types.
// It is used by several parts of the standard library as the base class for trait types,
// especially in their bool variant: see true_type and false_type.

// * How to use : integral_constant<bool, false>::value --> true or false
template <class _Tp, _Tp __v>
struct integral_constant
{
  static const _Tp      		value = __v;
  typedef _Tp               	value_type;
  typedef integral_constant		type;

  operator value_type() const {return value;}
};

typedef integral_constant<bool,true>	true_type;
typedef integral_constant<bool,false>	false_type;

// * (2) std::is_integral : [ Defined in header <type_traits> ]
// https://cplusplus.com/reference/type_traits/is_integral/?kw=is_integral
// 아래 특수화 케이스에 걸리는 애들은 모두 true, 나머지는 모두 false.
template <class _Tp> struct is_integral                     : public false_type {};
template <>          struct is_integral<bool>               : public true_type {};
template <>          struct is_integral<char>               : public true_type {};
template <>          struct is_integral<signed char>        : public true_type {};
template <>          struct is_integral<unsigned char>      : public true_type {};
template <>          struct is_integral<wchar_t>            : public true_type {};
template <>          struct is_integral<char16_t>           : public true_type {};
template <>          struct is_integral<char32_t>           : public true_type {};
template <>          struct is_integral<short>              : public true_type {};
template <>          struct is_integral<unsigned short>     : public true_type {};
template <>          struct is_integral<int>                : public true_type {};
template <>          struct is_integral<unsigned int>       : public true_type {};
template <>          struct is_integral<long>               : public true_type {};
template <>          struct is_integral<unsigned long>      : public true_type {};
template <>          struct is_integral<long long>          : public true_type {};
template <>          struct is_integral<unsigned long long> : public true_type {};

FT_END_GLOBAL_NAMESPACE

#endif
