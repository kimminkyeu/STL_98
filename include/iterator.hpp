#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP

namespace ft
{


// iterator 전체를 구현하지는 않고, reverse_iterator와 iterator_traits만 구현함.
template <typename T>
class reverse_iterator
{



};

// ? ? ? ? ? ? ? ? 
template <class Iterator>
struct iterator_traits
{
   typedef typename Iterator::iterator_category     iterator_category;
   typedef typename Iterator::value_type            value_type;
   typedef typename Iterator::difference_type       difference_type;
   typedef difference_type                          distance_type;
   typedef typename Iterator::pointer 				pointer;
   typedef typename Iterator::reference 			reference;
};

template <typename T>
struct iterator_traits<T*>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;
    typedef std::ptrdiff_t                  difference_type;
};


}

#endif