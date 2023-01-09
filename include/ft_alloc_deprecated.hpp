//
// Created by 김민규 on 2023/01/06.
//

#ifndef FT_CONTAINER_ALLOC_HPP
#define FT_CONTAINER_ALLOC_HPP

#include <cstddef>
#include <memory>
#include "__config.hpp"

FT_BEGIN_GLOBAL_NAMESPACE

// !--------------------------------------------------------------
// !                                                             |
// !          API Evolution and Deprecation History              |
// !                                                             |
// !---------------------------------------------------------------
// Ref 1 : https://github.com/lattera/openbsd/blob/master/gnu/lib/libstdc%2B%2B/libstdc%2B%2B/include/bits/stl_alloc.h
// Ref 2 : https://gcc.gnu.org/onlinedocs/gcc-8.3.0/libstdc++/manual/manual/api.html

// ! Log : 기껏 옛날 버전 찾아서 구조 연구하면서 왜 그럴까.. 고민 겁나 했는디, 결국 deprecated된 기능이였다...
// ! S_instanceless 같은 거 말이다...

// ! Previously, all allocators were written to the SGI style, and all STL containers expected this interface.
// ! This interface had a traits class called _Alloc_traits that attempted to provide more information
// ! for compile-time allocation selection and optimization.

// ! This traits class had another allocator wrapper, __simple_alloc<T,A>, which was a wrapper around -
// ! another allocator, A, which itself is an allocator for instances of T.

// ! But wait, there's more: __allocator<T,A> is another adapter.
// ! Many of the provided allocator classes were SGI style: such classes can be changed to a -
// ! conforming interface with this wrapper: __allocator<T, __alloc> is thus the same as allocator<T>.













// ---------------------------------------------------------------
// |                                                             |
// |              simple_alloc  implementation                   |
// |                                                             |
// ---------------------------------------------------------------
// #include "../libstdc++-2.90.8/stl/bits/stl_alloc.h"

// * 이 class는 객체 instance를 만들 필요가 없는 경우, r-value로 객체를 생성해서 해당 함수를 호출한다.
template<class Tp, class Alloc>
class simple_alloc
{
public:
    static Tp* allocate(size_t _n)
    {
      return (0 == _n ? 0 : (Tp*) Alloc::allocate(_n * sizeof (Tp)));
    }

    static Tp* allocate()
    {
      return (Tp*) Alloc::allocate(sizeof (Tp));
    }

    static void deallocate(Tp* _p, size_t _n)
    {
      if (0 != _n)
      {
        Alloc::deallocate(_p, _n * sizeof (Tp));
      }
    }

    static void deallocate(Tp* _p)
    {
      Alloc::deallocate(_p, sizeof (Tp));
    }
};



// ---------------------------------------------------------------
// |                                                             |
// |              [      allocator adaptor     ]                 |
// |              _Alloc_traits_  implementation                 |
// |                                                             |
// ---------------------------------------------------------------
// This serves two purposes.
// First, make it possible to write containers that can use either SGI-style allocators or standard-conforming allocator.
// Second, provide a mechanism so that containers can query whether or not the allocator has distinct instances.
// If not, the container can avoid wasting a word of memory to store an empty object.

// This adaptor uses partial specialization.  The general case of
// _Alloc_traits<_Tp, _Alloc> assumes that _Alloc is a standard-conforming allocator, possibly with non-equal instances and non-static members.
// (It still behaves correctly even if _Alloc has static member and if all instances are equal. Refinements affect performance, not correctness.)

// There are always two members: allocator_type, which is a standard-conforming allocator type
// for allocating objects of type _Tp, and _S_instanceless, a static const member of type bool.
// If _S_instanceless is true, this means that there is no difference between any two instances of type allocator_type.

// Furthermore, if _S_instanceless is true, then _Alloc_traits has one additional member: [_Alloc_type.]
// This type encapsulates allocation and deallocation of objects of type _Tp through a static interface;
// it has two member functions, whose signatures are
//    - static _Tp* allocate(size_t)
//    - static void deallocate(_Tp*, size_t)

// * 만약 std::allocator를 쓴다면, 아래의 코드는 S_intancess가 true인 코드로 생성된다.
// * 그게 아니라 custom allocator를 쓴다면, 데이터 멤버로 allocator instance가 들어온다.

// (1) The fully general version.
template <class Tp, class Allocator>
struct allocator_traits
{
    static const bool S_instanceless = false;
    typedef typename Allocator::template rebind<Tp>::other allocator_type;
};

template <class Tp, class Allocator>
const bool allocator_traits<Tp, Allocator>::S_instanceless;

// (2) The version for the default allocator.
// * if std::allocator, then allocator is instanceless.
// * which means container doesn't hold instance of allocator.
template <class Tp, class Tp1>
struct allocator_traits<Tp, std::allocator<Tp1> > // only when std::allocator, then true
{
    static const bool S_instanceless    = true;
    typedef typename std::allocator<Tp1>  alloc;
    typedef std::allocator<Tp>            allocator_type;

    // ? ------------------------------------
    typedef simple_alloc<Tp, alloc>       Alloc_type; // use allocator's alloc.
    // ? ------------------------------------
};




FT_END_GLOBAL_NAMESPACE

#endif
