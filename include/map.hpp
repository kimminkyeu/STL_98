//
// Created by 김민규 on 2023/01/23.
//


#ifndef FT_CONTAINER_MAP_HPP

#include <functional> // for std::less
#include "iterator.hpp"
#include "__Left_Leaning_RedBlack.hpp"
#include "__config.hpp"


FT_BEGIN_GLOBAL_NAMESPACE


template <class Key, class Value, class Compare = std::less<Key> >
class map_value_compare
{
public:
    typedef ft::pair<Key, Value> value_type;

protected:
    Compare comp;

    map_value_compare(Compare c)
        : comp(c)
    {}

public:

    bool operator()(const value_type &x, const value_type &y) const
    {
        return (comp(x.first, y.first));
    }
};


template <class Key, class Value, class Compare = std::less<Key>,
          class Allocator = std::allocator<FT::pair<const Key, Value> > >
class FT_TEMPLATE_VIS map
{
public:
    // types:
    typedef Key                                      key_type;         // 각 map 요소에 저장된 정렬 키에 대한 typede
    typedef Value                                    mapped_type;      // 각 map 요소에 저장된 데이터에 대한 typedef
    typedef pair<const key_type, mapped_type>        value_type;       // map의 요소로 저장된 개체의 형식에 대한 typedef
    typedef Compare                                  key_compare;      // map의 두 요소간 상대적 순서를 결정하는 두 정렬 키를 비교할 수 있는 함수 개체에 대한 typedef
    typedef Allocator                                allocator_type;   // map 개체를 위한 allocator 클래스의 typedef
    typedef typename allocator_type::reference       reference;        // map에 저장된 요소에 대한 참조의 typedef
    typedef typename allocator_type::const_reference const_reference;  // 읽기 및 const 작업을 수행하기 위해 const 맵에 저장된 요소에 대한 참조에 대한 typedef입니다.
    typedef typename allocator_type::pointer         pointer;          // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::const_pointer   const_pointer;    // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::size_type       size_type;        // map의 요소 수에 대한 부호 없는 정수의 typedef
    typedef typename allocator_type::difference_type difference_type;  // 반복기가 가리키는 요소 사이의 범위에 있는 map의 요소 개수에 대한 부호 있는 정수 typedef

//    typedef implementation-defined                   iterator;
//    typedef implementation-defined                   const_iterator;
//    typedef FT::reverse_iterator<iterator>          reverse_iterator;
//    typedef FT::reverse_iterator<const_iterator>    const_reverse_iterator;




};


FT_END_GLOBAL_NAMESPACE

#define FT_CONTAINER_MAP_HPP

#endif //FT_CONTAINER_MAP_HPP
