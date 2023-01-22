//
// Created by 김민규 on 2022/12/28.
//

#ifndef FT_CONTAINER_CONFIG_HPP
#define FT_CONTAINER_CONFIG_HPP

// for printing color ascii...
#define PRINT_RED     "\x1b[31m"
#define PRINT_GREEN   "\x1b[32m"
#define PRINT_YELLOW  "\x1b[33m"
#define PRINT_BLUE    "\x1b[34m"
#define PRINT_MAGENTA "\x1b[35m"
#define PRINT_CYAN    "\x1b[36m"
#define PRINT_RESET   "\x1b[0m"

//   --------------------------------------------------------------
//   |  하단의 내용은 llvm __config 파일 내용을 참고하여 작성하였습니다.     |
//   |                                                            |
// * |  https://libcxx.llvm.org/DesignDocs/VisibilityMacros.html  |
//   |                                                            |
//   --------------------------------------------------------------


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

#ifdef __cplusplus

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

#if __has_attribute(internal_linkage)
#  define FT_INTERNAL_LINKAGE __attribute__ ((internal_linkage))
#else
#  define FT_INTERNAL_LINKAGE _LIBCPP_ALWAYS_INLINE
#endif

// ---------------------------------------------------------------------------------------------
// * setting inline attributes

// * FT_ALWAYS_INLINE
/* - Ignore -fno-inline (this is what the documentation says).
   - Ignore the inlining limits hence inlining the function regardless. It also inlines functions with alloca calls, which inline keyword never does.
   - Not produce an external definition of a function with external linkage if marked with always_inline. */
#define FT_ALWAYS_INLINE    inline __attribute__ ((__always_inline__))

// * FT_INTERNAL_LINKAGE
// Ref : https://learn.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=msvc-170
#if __has_attribute(internal_linkage)
#  define FT_INTERNAL_LINKAGE   __attribute__ ((internal_linkage))
#else
#  define FT_INTERNAL_LINKAGE FT_ALWAYS_INLINE
#endif

// * FT_HIDDEN // (1) hidden일 경우 해당 함수가 라이브러리 외부로 노출되지 않는다.
// Mark a symbol as hidden so it will not be exported from shared libraries.
#ifndef FT_HIDDEN
#  if !defined(FT_DISABLE_VISIBILITY_ANNOTATIONS)
#    define FT_HIDDEN __attribute__ ((__visibility__("hidden")))
#  else
#    define FT_HIDDEN
#  endif
#endif

// @ function visibility in shared library
#ifndef FT_FUNC_VIS
#  if !defined(FT_DISABLE_VISIBILITY_ANNOTATIONS)
      // Mark a symbol as being exported by the libc++ library.
      // This attribute must be applied to the declaration of all functions exported by the libc++ dylib.
#    define FT_FUNC_VIS __attribute__ ((__visibility__("default")))
#  else
#    define FT_FUNC_VIS
#  endif
#endif

// @ type visibility in shared library
#ifndef FT_TYPE_VIS
#  if !defined(FT_DISABLE_VISIBILITY_ANNOTATIONS)
      // Mark a type’s typeinfo, vtable and members as having default visibility.
      // This attribute cannot be used on class templates.
#    define FT_TYPE_VIS __attribute__ ((__visibility__("default")))
#  else
#    define FT_TYPE_VIS
#  endif
#endif

// @ template visibility in shared library
#ifndef FT_TEMPLATE_VIS
#  if !defined(FT_DISABLE_VISIBILITY_ANNOTATIONS)
#    if __has_attribute(__type_visibility__)
      // Mark a type’s typeinfo and vtable as having default visibility.
      // This macro has no effect on the visibility of the type’s member functions.
      // GCC Behavior: GCC does not support Clang’s type_visibility(…) attribute. With GCC the visibility(…) attribute is used and member functions are affected.
      // Windows Behavior: DLLs do not support dllimport/export on class templates. The macro has an empty definition on this platform.
#      define FT_TEMPLATE_VIS __attribute__ ((__type_visibility__("default")))
#    else
#      define FT_TEMPLATE_VIS __attribute__ ((__visibility__("default")))
#    endif
#  else
#    define FT_TEMPLATE_VIS
#  endif
#endif

#define FT_HIDE_FROM_ABI_PER_TU_BY_DEFAULT

// TU = Translate Unit
#ifndef FT_HIDE_FROM_ABI_PER_TU
#  ifndef FT_HIDE_FROM_ABI_PER_TU_BY_DEFAULT
#    define FT_HIDE_FROM_ABI_PER_TU 0
#  else
#    define FT_HIDE_FROM_ABI_PER_TU 1
#  endif
#endif

// Mark a function as not being part of the ABI of any final linked image that uses it.
#ifndef FT_HIDE_FROM_ABI
#  if FT_HIDE_FROM_ABI_PER_TU
#    define FT_HIDE_FROM_ABI FT_HIDDEN FT_INTERNAL_LINKAGE
#  else
#    define FT_HIDE_FROM_ABI FT_HIDDEN FT_EXCLUDE_FROM_EXPLICIT_INSTANTIATION
#  endif
#endif


// * Deprecation macros.
// Deprecations warnings are always enabled, except when users explicitly opt-out
// by defining FT_DISABLE_DEPRECATION_WARNINGS.
#if !defined(FT_DISABLE_DEPRECATION_WARNINGS)
#  if __has_attribute(deprecated)
#    define FT_DEPRECATED __attribute__ ((deprecated))
#  elif FT_STD_VER > 11
#    define FT_DEPRECATED [[deprecated]]
#  else
#    define FT_DEPRECATED
#  endif
#else
#  define FT_DEPRECATED
#endif



// *--------------------------------------------------------------------------------------------
// Historical predecessor of _LIBCPP_HIDE_FROM_ABI – please use _LIBCPP_HIDE_FROM_ABI instead.
#define FT_INLINE_VISIBILITY FT_HIDE_FROM_ABI
// *--------------------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------------
// * because C++98 has no "noexcept" keyword ...
#if !(__has_feature(cxx_noexcept))
#define FT_HAS_NO_NOEXCEPT
#endif
#ifndef FT_HAS_NO_NOEXCEPT
#  define _NOEXCEPT noexcept
#  define _NOEXCEPT_(x) noexcept(x)
#else
#  define _NOEXCEPT throw()
#  define _NOEXCEPT_(x) // just for noexcept(x). does nothing in C++98 version
#endif


// * Fallthrough macro in Switch-Case
// Use a function like macro to imply that it must be followed by a semicolon
#if __cplusplus > 201402L && __has_cpp_attribute(fallthrough)
#  define _FALLTHROUGH() [[fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#  define _FALLTHROUGH() [[clang::fallthrough]]
#elif __has_attribute(fallthrough) || _GNUC_VER >= 700
#  define _FALLTHROUGH() __attribute__((__fallthrough__))
#else
#  define _FALLTHROUGH() ((void)0)
#endif



#endif // __cplusplus

#endif //FT_CONTAINER___CONFIG_HPP
