//
// Created by 김민규 on 2023/01/06.
//

#ifndef FT_CONTAINER_FT_VECTOR_BASE_HPP
#define FT_CONTAINER_FT_VECTOR_BASE_HPP

#include <cstddef>
#include <memory>
#include "__config.hpp"

FT_BEGIN_PRIVATE_NAMESPACE

// ---------------------------------------------------------------
// |                                                             |
// |              simple_alloc  implementation                   |
// |                                                             |
// ---------------------------------------------------------------
// * #include "../libstdc++-2.90.8/stl/bits/stl_alloc.h"

// ? vector_base 구현시 사용하는 것 때문에 가져왔으나, 추가적 분석이 필요함.
// ? (1) 왜 이렇게 감싸는 건지, 목적과 이유를 알아낼 것.

// ? What is S_instanceless ?

template<class Tp, class Alloc>
class simple_alloc
{
public:
    static Tp* allocate(size_t _n)
    { return 0 == _n ? 0 : (Tp*) Alloc::allocate(_n * sizeof (Tp)); }

    static Tp* allocate()
    { return (Tp*) Alloc::allocate(sizeof (Tp)); }

    static void deallocate(Tp* _p, size_t _n)
    { if (0 != _n) Alloc::deallocate(_p, _n * sizeof (Tp)); }

    static void deallocate(Tp* _p)
    { Alloc::deallocate(_p, sizeof (Tp)); }
};

// Another allocator adaptor: _Alloc_traits.  This serves two
// purposes.  First, make it possible to write containers that can use
// either SGI-style allocators or standard-conforming allocator.
// Second, provide a mechanism so that containers can query whether or
// not the allocator has distinct instances.  If not, the container
// can avoid wasting a word of memory to store an empty object.

// This adaptor uses partial specialization.  The general case of
// _Alloc_traits<_Tp, _Alloc> assumes that _Alloc is a
// standard-conforming allocator, possibly with non-equal instances
// and non-static members.  (It still behaves correctly even if _Alloc
// has static member and if all instances are equal.  Refinements
// affect performance, not correctness.)

// There are always two members: allocator_type, which is a standard-
// conforming allocator type for allocating objects of type _Tp, and
// _S_instanceless, a static const member of type bool.  If
// _S_instanceless is true, this means that there is no difference
// between any two instances of type allocator_type.  Furthermore, if
// _S_instanceless is true, then _Alloc_traits has one additional
// member: _Alloc_type.  This type encapsulates allocation and
// deallocation of objects of type _Tp through a static interface; it
// has two member functions, whose signatures are
//    static _Tp* allocate(size_t)
//    static void deallocate(_Tp*, size_t)

// The fully general version.

template <class Tp, class Allocator>
struct Alloc_traits
{
    static const bool S_instanceless = false;
    typedef typename Allocator::template rebind<Tp>::other allocator_type;
};

template <class Tp, class Allocator>
const bool Alloc_traits<Tp, Allocator>::S_instanceless;

// The version for the default allocator.

template <class Tp, class Tp1>
struct Alloc_traits<Tp, std::allocator<Tp1> >
{
    typedef typename std::allocator<Tp1> alloc;
    static const bool S_instanceless = true;
    typedef simple_alloc<Tp, alloc> Alloc_type;
    typedef std::allocator<Tp> allocator_type;
};

// Versions for the predefined SGI-style allocators.

//template <class _Tp, int __inst>
//struct Alloc_traits<_Tp, __malloc_alloc_template<__inst> >
//{
//static const bool _S_instanceless = true;
//typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
//typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
//};


//template <class _Tp, class _Alloc>
//struct _Alloc_traits<_Tp, debug_alloc<_Alloc> >
//{
//static const bool _S_instanceless = true;
//typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
//typedef __allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
//};

// Versions for the __allocator adaptor used with the predefined
// SGI-style allocators.

//template <class _Tp, class _Tp1, int __inst>
//struct _Alloc_traits<_Tp,
//        __allocator<_Tp1, __malloc_alloc_template<__inst> > >
//{
//static const bool _S_instanceless = true;
//typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
//typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
//};
//
//#ifndef __USE_MALLOC
//template <class _Tp, class _Tp1, bool __thr, int __inst>
//struct _Alloc_traits<_Tp,
//        __allocator<_Tp1,
//        __default_alloc_template<__thr, __inst> > >
//{
//static const bool _S_instanceless = true;
//typedef simple_alloc<_Tp, __default_alloc_template<__thr,__inst> >
//        _Alloc_type;
//typedef __allocator<_Tp, __default_alloc_template<__thr,__inst> >
//        allocator_type;
//};
//#endif





// ---------------------------------------------------------------
// |                                                             |
// |                Vector-Base implementation                   |
// |                                                             |
// ---------------------------------------------------------------
// The vector base class serves two purposes.
// First, its constructor and destructor allocate (but don't initialize) storage.
// * This makes exception safety easier.
// Second, the base class encapsulates all the differences between SGI-style allocators
// and standard-conforming allocators.

#ifdef FT_USE_STD_ALLOCATORS

// Base class for ordinary allocators.
template <class Tp, class Allocator, bool IsStatic>
class Vector_alloc_base {
public:
  typedef typename Alloc_traits<Tp, Allocator>::allocator_type allocator_type;

  allocator_type get_allocator() const
  { return M_data_allocator; }

  Vector_alloc_base(const allocator_type& _a)
    : M_data_allocator(_a), M_start(0), M_finish(0), M_end_of_storage(0)
  {}

protected:
  allocator_type M_data_allocator;
  Tp* M_start;
  Tp* M_finish;
  Tp* M_end_of_storage;

  Tp* M_allocate(size_t _n)
    { return M_data_allocator.allocate(_n); }
  void M_deallocate(Tp* _p, size_t _n)
    { if (_p) M_data_allocator.deallocate(_p, _n); }
};

// Specialization for allocators that have the property that we don't
// actually have to store an allocator object.
template <class Tp, class Allocator>
class Vector_alloc_base<Tp, Allocator, true> {
public:
  typedef typename Alloc_traits<Tp, Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  Vector_alloc_base(const allocator_type&)
    : M_start(0), M_finish(0), M_end_of_storage(0)
  {}

protected:
  Tp* M_start;
  Tp* M_finish;
  Tp* M_end_of_storage;

  typedef typename Alloc_traits<Tp, Allocator>::_Alloc_type Alloc_type;
  Tp* M_allocate(size_t _n)
    { return Alloc_type::allocate(_n); }
  void _M_deallocate(Tp* _p, size_t _n)
    { Alloc_type::deallocate(_p, _n);}
};

template <class Tp, class Alloc>
struct Vector_base
  : public Vector_alloc_base<Tp, Alloc,
                              Alloc_traits<Tp, Alloc>::_S_instanceless>
{
  typedef Vector_alloc_base<Tp, Alloc,
                             Alloc_traits<Tp, Alloc>::_S_instanceless>
          Base;
  typedef typename Base::allocator_type allocator_type;

  Vector_base(const allocator_type& _a) : Base(_a) {}
  Vector_base(size_t _n, const allocator_type& _a) : Base(_a) {
    M_start = M_allocate(__n);
    M_finish = M_start;
    M_end_of_storage = M_start + __n;
  }

  ~Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
};

#else /* __STL_USE_STD_ALLOCATORS */

template <class Tp, class Alloc>
class Vector_base
{
public:
    typedef Alloc allocator_type;

    allocator_type get_allocator() const
    { return allocator_type(); }

    explicit Vector_base(const Alloc&)
            : M_start(0), M_finish(0), M_end_of_storage(0)
    {}

    Vector_base(size_t _n, const Alloc&)
            : M_start(0), M_finish(0), M_end_of_storage(0)
    {
        M_start = M_allocate(_n);
        M_finish = M_start;
        M_end_of_storage = M_start + _n;
    }

    ~Vector_base()
    { _M_deallocate(M_start, M_end_of_storage - M_start); }

protected:
    Tp* M_start;
    Tp* M_finish;
    Tp* M_end_of_storage;

    typedef simple_alloc<Tp, Alloc> M_data_allocator;

    Tp* M_allocate(size_t _n)
    { return M_data_allocator::allocate(_n); }

    void M_deallocate(Tp* _p, size_t _n)
    { M_data_allocator::deallocate(_p, _n); }
};

#endif /* FT_USE_STD_ALLOCATORS */

FT_END_PRIVATE_NAMESPACE

#endif //FT_CONTAINER_FT_VECTOR_BASE_HPP
