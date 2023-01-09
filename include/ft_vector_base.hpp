//
// Created by 김민규 on 2023/01/06.
//

#ifndef FT_CONTAINER_FT_VECTOR_BASE_HPP
#define FT_CONTAINER_FT_VECTOR_BASE_HPP

// #include <cstddef>
// #include <memory>
#include "__config.hpp"
#include "ft_alloc.hpp"

FT_BEGIN_PRIVATE_NAMESPACE

// ---------------------------------------------------------------
// |                                                             |
// |           Vector-alloc-Base implementation (1)              |
// |                                                             |
// ---------------------------------------------------------------
// Base class for ordinary allocators.
template <class Tp, class Allocator, bool IsStatic>
class Vector_alloc_base {
public:
  typedef typename FT::allocator_traits<Tp, Allocator>::allocator_type  allocator_type;

  allocator_type get_allocator() const
  {
    return m_Data_allocator;
  }

  Vector_alloc_base(const allocator_type& _a)
    : m_Data_allocator(_a), m_Start(0), m_Finish(0), m_End_of_storage(0)
  {}

protected: // * if allocator is instance-full.
  allocator_type m_Data_allocator;
  Tp* m_Start;
  Tp* m_Finish;
  Tp* m_End_of_storage;

  Tp* m_Allocate(size_t _n)
    {
      return m_Data_allocator.allocate(_n);
    }

  void m_Deallocate(Tp* _p, size_t _n)
    {
      if (_p)
      {
        m_Data_allocator.deallocate(_p, _n);
      }
    }
};


// ---------------------------------------------------------------
// |                                                             |
// |           Vector-alloc-Base implementation (2)              |
// |                                                             |
// ---------------------------------------------------------------
// * -----------------------------------------
// * [       if std::allocator is used.      ]
// * -----------------------------------------
// * Specialization for allocators that have the property
// * that we don't have to store an allocator instance.
template <class Tp, class Allocator>
class Vector_alloc_base<Tp, Allocator, true>
      // * std::allocator를 사용할 경우, Alloc_traits의 S_instanceless는 true를 반환함.
{
public:
  typedef typename FT::allocator_traits<Tp, Allocator>::allocator_type      allocator_type;

  allocator_type get_allocator() const
  {
    return allocator_type(); // * return r-value instance of allocator.
  }

  Vector_alloc_base(const allocator_type&)
    : m_Start(0), m_Finish(0), m_End_of_storage(0)
  {}

protected:
  Tp* m_Start;
  Tp* m_Finish;
  Tp* m_End_of_storage;

protected:
  Tp* m_allocate(size_t _n)
  {
    return allocator_type::allocate(_n);
  }

  void m_deallocate(Tp* _p, size_t _n)
  {
    allocator_type::deallocate(_p, _n);
  }
};



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


// if given Alloc is instanceless, then call static allocator functions.
template <class Tp, class Alloc>
class Vector_base
  : public Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::_S_instanceless>
{

public:
  typedef Vector_alloc_base<Tp, Alloc, FT::allocator_traits<Tp, Alloc>::S_instanceless>     Base;

  typedef typename Base::allocator_type                                                     allocator_type;

  Vector_base(const allocator_type& _a)
    : Base(_a)
  {}

  Vector_base(size_t _n, const allocator_type& _a)
    : Base(_a)
  {
    this->m_Start = this->m_allocate(_n);
    this->m_Finish = this->m_Start;
    this->m_End_of_storage = this->m_Start + _n;
  }

  ~Vector_base()
  {
    m_deallocate(this->m_Start, this->m_End_of_storage - this->m_Start);
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }

};

FT_END_PRIVATE_NAMESPACE

#endif //FT_CONTAINER_FT_VECTOR_BASE_HPP
