//
// Created by 김민규 on 2023/01/23.
//


#ifndef FT_CONTAINER_MAP_HPP

#include <memory>
#include <functional> // for std::less
#include "iterator.hpp"
#include "__Left_Leaning_RedBlack.hpp"
#include "__config.hpp"




// ---------------------------------------------------------------
// |                                                             |
// |                   map implementation                        |
// |                                                             |
// ---------------------------------------------------------------
// * map은 LeftLeaningRedBlack class의 adapter class이며, 내부 로직은 모두 __tree__ 멤버 데이터를 통해 이뤄집니다.


FT_BEGIN_GLOBAL_NAMESPACE

template <class Key, class Value, class Compare = std::less<Key>,
          class Allocator = std::allocator<FT::pair<const Key, Value> > >
class FT_TEMPLATE_VIS map
{
public:
    // Member types:
    typedef Key                                         key_type;         // 각 map 요소에 저장된 정렬 키에 대한 typedef
    typedef Value                                       mapped_type;      // 각 map 요소에 저장된 데이터에 대한 typedef
    typedef pair<const key_type, mapped_type>           value_type;       // map의 요소로 저장된 개체의 형식에 대한 typedef

    typedef Compare                                     key_compare;      // map의 두 요소간 상대적 순서를 결정하는 두 정렬 키를 비교할 수 있는 함수 개체에 대한 typedef
    typedef Allocator                                   allocator_type;   // map 개체를 위한 allocator 클래스의 typedef
    typedef value_type&                                 reference;        // map에 저장된 요소에 대한 참조의 typedef
    typedef const value_type&                           const_reference;  // 읽기 및 const 작업을 수행하기 위해 const 맵에 저장된 요소에 대한 참조에 대한 typedef

    // map은 둘다 const이다. 왜냐면 Key에 const가 무조건 붙어서 들어가기 때문이다.
    typedef typename allocator_type::pointer            pointer;          // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::const_pointer      const_pointer;    // map의 const 요소를 가리키는 포인터에 대한 typedef
    typedef typename allocator_type::size_type          size_type;        // map의 요소 수에 대한 부호 없는 정수의 typedef
    typedef typename allocator_type::difference_type    difference_type;  // 반복기가 가리키는 요소 사이의 범위에 있는 map의 요소 개수에 대한 부호 있는 정수 typedef

public:
    // Member classes: a function object for map
    // Ref : https://en.cppreference.com/w/cpp/container/map/value_compare
    class value_compare : public std::binary_function<value_type, value_type, bool>
    {
    friend class map<key_type, mapped_type, key_compare, allocator_type>;

    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}

    public:
        bool operator()(const value_type &x, const value_type &y) const
        {
            return (comp(x.first, y.first));
        }
    };

private:
    typedef _PRIVATE::LeftLeaningRedBlack<value_type, value_compare>        _map_base;

public:
    typedef typename _map_base::iterator                                    iterator;
    typedef typename _map_base::const_iterator                              const_iterator;
    typedef typename FT::reverse_iterator<iterator>                         reverse_iterator;
    typedef typename FT::reverse_iterator<const_iterator>                   const_reverse_iterator;


private: // Member data:
// * -------------------------------------------------
    _map_base   __tree__;
// * -------------------------------------------------


public: // constructor & destructor.

    map(const key_compare& comp = key_compare(), const allocator_type& a = allocator_type())
        : __tree__(comp, a)
        {}


    template <class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& comp = key_compare());

    template <class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& comp, const allocator_type& a);

    map(const map& m);

    explicit map(const allocator_type& a);

    map(const map& m, const allocator_type& a);

   ~map();

    map& operator=(const map& m);

    allocator_type get_allocator() const;

public:
    // iterators:
  iterator begin() _NOEXCEPT;
  const_iterator begin() const _NOEXCEPT;
  iterator end() _NOEXCEPT;
  const_iterator end() const _NOEXCEPT;
  reverse_iterator rbegin() _NOEXCEPT;
  const_reverse_iterator rbegin() const _NOEXCEPT;
  reverse_iterator rend() _NOEXCEPT;
  const_reverse_iterator rend() const _NOEXCEPT;

public:
    // capacity:
    bool      empty()    const _NOEXCEPT;
    size_type size()     const _NOEXCEPT;
    size_type max_size() const _NOEXCEPT;

public:
    // element access:
    mapped_type& operator[](const key_type& k);
    mapped_type& at(const key_type& k);
    const mapped_type& at(const key_type& k) const;

public:
    // modifiers:
    FT::pair<iterator, bool> insert(const value_type& v);

    iterator insert(const_iterator position, const value_type& v);

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);

    void clear();

void erase( iterator pos );
void erase( iterator first, iterator last );
size_type erase( const Key& key );
void swap( map& other );

public:
    // Lookup:
size_type count( const Key& key ) const;
iterator find( const Key& key );
const_iterator find( const Key& key ) const;

FT::pair<iterator,iterator> equal_range( const Key& key );
FT::pair<const_iterator,const_iterator> equal_range( const Key& key ) const;

iterator lower_bound( const Key& key );
const_iterator lower_bound( const Key& key ) const;

iterator upper_bound( const Key& key );
const_iterator upper_bound( const Key& key ) const;

public:
    // Observers:
    key_compare key_comp() const;
    value_compare value_comp() const;

};

// Non-member functions:
template <class Key, class T, class Compare, class Allocator>
bool
operator==(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator< (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator!=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator> (const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator>=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool
operator<=(const map<Key, T, Compare, Allocator>& x,
           const map<Key, T, Compare, Allocator>& y);

// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void
swap(map<Key, T, Compare, Allocator>& x, map<Key, T, Compare, Allocator>& y); /* (C++17) _NOEXCEPT_(_NOEXCEPT_(x.swap(y))) */

FT_END_GLOBAL_NAMESPACE

#define FT_CONTAINER_MAP_HPP

#endif //FT_CONTAINER_MAP_HPP
