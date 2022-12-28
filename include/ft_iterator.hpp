#pragma once
#ifndef FT_CONTAINER_ITERATOR_HPP
#define FT_CONTAINER_ITERATOR_HPP


// std::iterator를 참조할 것.
#include <iterator>

// NOTE: iterator를 직접 작성한 이유는, 공부를 위해서이다.
// [참조] https://www.youtube.com/watch?v=F9eDv-YIOQ0&t=976s

namespace ft
{


template<typename T>
class iterator
{};

template<typename T>
class reverse_iterator
{};

template <typename T>
struct iterator_traits
{};




}

#endif