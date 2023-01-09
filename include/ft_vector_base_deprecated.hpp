//
// Created by 김민규 on 2023/01/06.
//

#ifndef FT_CONTAINER_FT_VECTOR_BASE_DEPRECATED_HPP
#define FT_CONTAINER_FT_VECTOR_BASE_DEPRECATED_HPP

#include "__config.hpp"
#include "ft_alloc_deprecated.hpp"

FT_BEGIN_PRIVATE_NAMESPACE


// ---------------------------------------------------------------
// |                                                             |
// |           Vector-alloc-Base implementation (1)              |
// |                                                             |
// ---------------------------------------------------------------
// Base class for ordinary allocators.
template <class Tp, class Allocator, bool IsStatic>
class Vector_alloc_base
{
public:
  typedef typename FT::allocator_traits<Tp, Allocator>::allocator_type  allocator_type;

  explicit Vector_alloc_base(const allocator_type& _a)
    : m_Data_allocator(_a), m_Start(0), m_Finish(0), m_End_of_storage(0)
  {}

protected: // * if allocator is instance-full.
    // ------------------------------
    allocator_type m_Data_allocator;
    // ------------------------------
    Tp* m_Start;
    Tp* m_Finish;
    Tp* m_End_of_storage;

    Tp* m_Allocate(size_t _n)
    {
        return m_Data_allocator.allocate(_n);
    }

    void m_Deallocate(Tp* _p, size_t _n)
    {
        if (_p) { m_Data_allocator.deallocate(_p, _n); }
    }

public:
    allocator_type get_allocator() const
    {
        return m_Data_allocator;
    }
};


// !--------------------------------------------------------------
// !           [ Deprecated ]                                    |
// !           Vector-alloc-Base implementation (2)              |
// !--------------------------------------------------------------
template <class Tp, class Allocator>
class Vector_alloc_base<Tp, Allocator, true>
{
public:
  typedef typename FT::allocator_traits<Tp, Allocator>::allocator_type      allocator_type;
  typedef typename FT::allocator_traits<Tp, Allocator>::Alloc_type          Alloc_type;

  explicit Vector_alloc_base(const allocator_type&)
    : m_Start(0), m_Finish(0), m_End_of_storage(0)
  {}

protected:
    Tp* m_Start;
    Tp* m_Finish;
    Tp* m_End_of_storage;

protected:
    Tp* m_Allocate(size_t _n)
    {
//        return allocator_type::allocate(_n);
        return Alloc_type::allocate(_n);
    }

    void m_Deallocate(Tp* _p, size_t _n)
    {
//        allocator_type::deallocate(_p, _n);
        Alloc_type::deallocate(_p, _n);
    }

public:
    allocator_type get_allocator() const
    {
        return allocator_type(); // * return r-value instance of allocator.
    }
};



// ---------------------------------------------------------------
// |                                                             |
// |                Vector-Base implementation                   |
// |                                                             |
// ---------------------------------------------------------------
// * NOTE : The vector base class serves two purposes.
// * First, its constructor and destructor allocate (but don't initialize) storage.
// * This makes exception safety easier.
// +) vector의 exception은 모두 allocate, deallocate에서 발생한다. 따라서 이 부분만 따로 class로 빼놓은 것이다.

// Second, the base class encapsulates all the differences between SGI-style allocators and standard-conforming allocators.
// An SGI STL allocator consists of a class with 3 required member functions, all of which are static.
// SGI는 boost library의 allocator 디자인으로, 메모리 할당/해제 함수가 static 이다.
// Ref : https://www.boost.org/sgi/stl/alloc.html


// if given Alloc is instanceless, then call static allocator functions.
template <class Tp, class Alloc>
class Vector_base
  : public Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::S_instanceless>
{

public:
  typedef Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::S_instanceless>     _Alloc_Base;
  typedef typename _Alloc_Base::allocator_type                                              allocator_type;

  explicit Vector_base(const allocator_type& _a)
    : _Alloc_Base(_a)
  {}

  explicit Vector_base(size_t _n, const allocator_type& _a)
    : _Alloc_Base(_a)
  {
      this->m_Start = this->m_allocate(_n);
      this->m_Finish = this->m_Start;
      this->m_End_of_storage = this->m_Start + _n;
  }

  ~Vector_base()
  {
      _Alloc_Base::m_Deallocate(this->m_Start, this->m_End_of_storage - this->m_Start);
  }
};

template <class Tp, class Alloc>
class Vector_base
  : public Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::S_instanceless> {

public:
    typedef Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::S_instanceless> _Alloc_Base;
    typedef typename _Alloc_Base::allocator_type allocator_type;

    explicit Vector_base(const allocator_type &_a)
            : _Alloc_Base(_a) {}

    explicit Vector_base(size_t _n, const allocator_type &_a)
            : _Alloc_Base(_a) {
        this->m_Start = this->m_allocate(_n);
        this->m_Finish = this->m_Start;
        this->m_End_of_storage = this->m_Start + _n;
    }

    ~Vector_base() {
        _Alloc_Base::m_Deallocate(this->m_Start, this->m_End_of_storage - this->m_Start);
    }
};


FT_END_PRIVATE_NAMESPACE

#endif //FT_CONTAINER_FT_VECTOR_BASE_DEPRECATED_HPP
