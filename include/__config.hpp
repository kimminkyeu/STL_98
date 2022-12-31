//
// Created by 김민규 on 2022/12/28.
//

#ifndef FT_CONTAINER_CONFIG_HPP
#define FT_CONTAINER_CONFIG_HPP

/* ---------------------------------------------------
   |  llvm __config 파일 내용을 참고하여 작성하였습니다.     |
   |                                                 |
   --------------------------------------------------*/

#ifdef __cplusplus

// ---------------------------------------------------------------------------------------------
// * handle namespace of library (ex. ft::vector, ft::map)
// @ namespace for library function call
#define FT                              ft
#define FT_BEGIN_GLOBAL_NAMESPACE       namespace FT {
#define FT_END_GLOBAL_NAMESPACE         }

// @ namespace for internal helper function call ( not for API )
//   - you can access helper functions by calling ft::_PRIVATE::foo()
#define _PRIVATE                        __this_is_hidden_namespace_for_internal_helper_function__
#define FT_BEGIN_PRIVATE_NAMESPACE      FT_BEGIN_GLOBAL_NAMESPACE namespace _PRIVATE {
#define FT_END_PRIVATE_NAMESPACE        } }


// ---------------------------------------------------------------------------------------------
//  * C++ version defined macro */
#ifndef FT_STD_VER
#  if   __cplusplus <= 199711L
#   define FT_STD_VER 1 // C++98
#  elif  __cplusplus <= 201103L
#    define FT_STD_VER 11
#  elif __cplusplus <= 201402L
#    define FT_STD_VER 14
#  elif __cplusplus <= 201703L
#    define FT_STD_VER 17
#  elif __cplusplus <= 202002L
#    define FT_STD_VER 20
#  else
#    define FT_STD_VER 21  // current year, or date of c++2b ratification
#  endif
#endif  // FT_STD_VER


// ---------------------------------------------------------------------------------------------
// * setting inline attributes

// @ FT_ALWAYS_INLINE
#define FT_ALWAYS_INLINE    __attribute__ ((__always_inline__))

// @ FT_INTERNAL_LINKAGE
// ? internal_linkage의 역할과 목적은?
#if __has_attribute(internal_linkage)
#  define FT_INTERNAL_LINKAGE __attribute__ ((internal_linkage))
#else
#  define FT_INTERNAL_LINKAGE FT_ALWAYS_INLINE
#endif

// @ FT_HIDDEN
// (1) hidden일 경우 해당 함수가 라이브러리 외부로 노출되지 않는다.
// ? 근데 언제, 아땋게 쓰이는지는 아직 모르겠음. --> 더 알아볼 것.
#ifndef FT_HIDDEN
#  if !defined(FT_DISABLE_VISIBILITY_ANNOTATIONS)
#    define FT_HIDDEN __attribute__ ((__visibility__("hidden")))
#  else
#    define FT_HIDDEN
#  endif
#endif

// * About Enable-if function. why???
/*
#ifndef FT_PREFERRED_OVERLOAD
#  if __has_attribute(__enable_if__)
#    define FT_PREFERRED_OVERLOAD __attribute__ ((__enable_if__(true, "")))
#  endif
#endif
 */

// * Deprecation macros.
// Deprecations warnings are always enabled, except when users explicitly opt-out
// by defining _LIBCPP_DISABLE_DEPRECATION_WARNINGS.
#if !defined(FT_DISABLE_DEPRECATION_WARNINGS)
#  if __has_attribute(deprecated)
#    define FT_DEPRECATED __attribute__ ((deprecated))
#  elif FT_STD_VER > 11
#    define _FT_DEPRECATED [[deprecated]]
#  else
#    define _FT_DEPRECATED
#  endif
#else
#  define _FT_DEPRECATED
#endif

// * set inline attribute here...
#define FT_INLINE_VISIBILITY    FT_ALWAYS_INLINE



// ---------------------------------------------------------------------------------------------
// * because C++98 has no "noexcept" keyword ...
#if !(__has_feature(cxx_noexcept))
#define FT_HAS_NO_NOEXCEPT
#endif
#ifndef FT_HAS_NO_NOEXCEPT
#  define _NOEXCEPT noexcept
#  define _NOEXCEPT_(x) noexcept(x)
#else
#  define FT_NOEXCEPT throw()
#  define FT_NOEXCEPT_(x)
#endif


// * Fallthrough macro in Switch-Case
// Use a function like macro to imply that it must be followed by a semicolon
#if __cplusplus > 201402L && __has_cpp_attribute(fallthrough)
#  define FT_FALLTHROUGH() [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#  define FT_FALLTHROUGH() [[clang::fallthrough]]
#elif __has_attribute(fallthrough) || _GNUC_VER >= 700
#  define FT_FALLTHROUGH() __attribute__((__fallthrough__))
#else
#  define FT_FALLTHROUGH() ((void)0)
#endif



#endif // __cplusplus

#endif //FT_CONTAINER___CONFIG_HPP