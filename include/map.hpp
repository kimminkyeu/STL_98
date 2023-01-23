//
// Created by 김민규 on 2023/01/23.
//


#ifndef FT_CONTAINER_MAP_HPP

#include <functional>
#include "iterator.hpp"
#include "__Left_Leaning_RedBlack.hpp"
#include "__config.hpp"

FT_BEGIN_GLOBAL_NAMESPACE

// _TreeIterator is [ RbNode * ]
template<typename _TreeIterator, typename Container>
class FT_TEMPLATE_VIS __map_iterator
{
private: // typedef from class Left_Leaning_Red_Black
    typedef typename _TreeIterator::node_type   node_type;

protected:
    _TreeIterator m_Current; // * 이때 들어오는 m_Current는 노드 포인터다.

public:
//    typedef typename FT::iterator_traits<_TreeIterator>     Traits;
    typedef typename std::bidirectional_iterator_tag               iterator_category;
    typedef typename _TreeIterator::key_type                       value_type;
    typedef typename _TreeIterator::difference_type                difference_type;
    typedef typename _TreeIterator::pointer                        pointer;
    typedef typename _TreeIterator::reference                      reference;

private:
    typedef __map_iterator<_TreeIterator, Container>               map_iterator_type;

public: // constructor & destructor

    const _TreeIterator& base() const
    { return m_Current; }

    __map_iterator()
            : m_Current()
    {}

    // constructor Wrapping
    explicit __map_iterator(const _TreeIterator& _iterator_in)
            : m_Current(_iterator_in)
    {}

    // copy constructor (used at casting)
    template<typename Iter>
    explicit __map_iterator(const __map_iterator<Iter, Container>& _other_iterator)
            : m_Current(_other_iterator.base()) // wrapper가 감싸고 있는 부분을 깊은 복사하는 것.
    {}

public: // operator

    //. Because of explicit keyword on constructor, i added const_iterator cast operator. ( Type-casting [T*] to [const T*] )
//    operator __map_iterator<const value_type*, Container>()
//    {
//        return __map_iterator<const value_type*, Container>(this->m_Current);
//    }

    reference operator*() const
    { return (*m_Current); }

    pointer operator->() const
    { return m_Current; }

    template<typename Iter>
    map_iterator_type& operator=(const __map_iterator<Iter, Container>& _other_iterator)
    {
        m_Current = _other_iterator.base();
        return (*this);
    }

    map_iterator_type operator++(int)
    { return map_iterator(m_Current++); }

    map_iterator_type& operator++()
    { ++m_Current; return *this; }

    map_iterator_type operator--(int)
    { return map_iterator(m_Current--); }

    map_iterator_type& operator--()
    { --m_Current; return *this; }
};

// @ forward iterator requirements.
// left-hand-side / right_hand_side

template<typename _IteratorL, typename _IteratorR, typename Container>
bool operator==(const __map_iterator<_IteratorL, Container>& __lhs,
                const __map_iterator<_IteratorR, Container>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename Container>
bool operator!=(const __map_iterator<_IteratorL, Container>& __lhs,
                const __map_iterator<_IteratorR, Container>& __rhs)
{ return !(__lhs == __rhs); }








template <class Key, class Value, class Compare = std::less<Key>,
          class Allocator = std::allocator<FT::pair<const Key, Value> > >
class FT_TEMPLATE_VIS map
{
public:
    // types:
    typedef Key                                      key_type; // 각 map 요소에 저장된 정렬 키에 대한 typede
    typedef Value                                    mapped_type; // 각 map 요소에 저장된 데이터에 대한 typedef
    typedef pair<const key_type, mapped_type>        value_type; // map의 요소로 저장된 개체의 형식에 대한 typedef
    typedef Compare                                  key_compare; // map의 두 요소간 상대적 순서를 결정하는 두 정렬 키를 비교할 수 있는 함수 개체에 대한 typedef
    typedef Allocator                                allocator_type; // map 개체를 위한 allocator 클래스의 typedef
    typedef typename allocator_type::reference       reference; // map에 저장된 요소에 대한 참조의 typedef
    typedef typename allocator_type::const_reference const_reference; // 읽기 및 const 작업을 수행하기 위해 const 맵에 저장된 요소에 대한 참조에 대한 typedef입니다.
    typedef typename allocator_type::pointer         pointer; // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::const_pointer   const_pointer; // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::size_type       size_type; // map의 요소 수에 대한 부호 없는 정수의 typedef
    typedef typename allocator_type::difference_type difference_type; // 반복기가 가리키는 요소 사이의 범위에 있는 map의 요소 개수에 대한 부호 있는 정수 typedef

//    typedef implementation-defined                   iterator;
//    typedef implementation-defined                   const_iterator;
//    typedef FT::reverse_iterator<iterator>          reverse_iterator;
//    typedef FT::reverse_iterator<const_iterator>    const_reverse_iterator;




};


FT_END_GLOBAL_NAMESPACE

#define FT_CONTAINER_MAP_HPP

#endif //FT_CONTAINER_MAP_HPP
