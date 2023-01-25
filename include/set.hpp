//
// Created by 김민규 on 2023/01/23.
//

#ifndef FT_CONTAINER_SET_HPP
#define FT_CONTAINER_SET_HPP



#include <iterator>
#include <memory>
#include <functional> // for std::less
#include "iterator.hpp"
#include "__Left_Leaning_RedBlack.hpp"
#include "__config.hpp"
#include "vector.hpp"

FT_BEGIN_PRIVATE_NAMESPACE

// ---------------------------------------------------------------
// |                                                             |
// |             set_const_iterator_implementation               |
// |                                                             |
// ---------------------------------------------------------------
// * Set은 모든 iterator가 const_iterator입니다. Key를 바꿀 수 없기 때문입니다.

template <typename _Key, typename _NodeType, class _TreeConstIterator = _PRIVATE::__tree_const_iterator<_Key, _NodeType> >
class FT_TEMPLATE_VIS __set_const_iterator
{
private:
    typedef typename _TreeConstIterator::node_type                      _node_type;
    typedef typename _PRIVATE::__tree_iterator<_Key, _NodeType>         _non_const_tree_iterator;

private: // data member
// * -------------------------------------------
    _TreeConstIterator __i_;
// * -------------------------------------------

private: // helper function
    _TreeConstIterator& base() const _NOEXCEPT
    { return __i_; }

public:
    typedef std::bidirectional_iterator_tag                      iterator_category;
    typedef typename _node_type::key_type                        value_type;            // pair
    typedef typename _TreeConstIterator::difference_type         difference_type;
    typedef typename _node_type::const_key_type_reference        reference;       // const pair &
    typedef typename _node_type::const_key_type_pointer          pointer;         // const pair *

public:
    FT_HIDE_FROM_ABI
    __set_const_iterator() _NOEXCEPT
    {}

    FT_HIDE_FROM_ABI
    __set_const_iterator(_TreeConstIterator __i) _NOEXCEPT
            : __i_(__i)
    {}

    FT_HIDE_FROM_ABI
    reference operator*() const
    { return reference(__i_->key); }

    FT_HIDE_FROM_ABI
    pointer operator->() const
    { return pointer(&(__i_->key)); }

    FT_HIDE_FROM_ABI
    __set_const_iterator& operator++() {++__i_; return *this;}

    FT_HIDE_FROM_ABI
    __set_const_iterator operator++(int)
    {
        __set_const_iterator __t(*this);
        ++(*this);
        return __t;
    }

    FT_HIDE_FROM_ABI
    __set_const_iterator& operator--() {--__i_; return *this;}

    FT_HIDE_FROM_ABI
    __set_const_iterator operator--(int)
    {
        __set_const_iterator __t(*this);
        --(*this);
        return __t;
    }

    friend FT_HIDE_FROM_ABI
    bool operator==(const __set_const_iterator& __x, const __set_const_iterator& __y)
    {return __x.__i_ == __y.__i_;}

    friend FT_HIDE_FROM_ABI
    bool operator!=(const __set_const_iterator& __x, const __set_const_iterator& __y)
    {return __x.__i_ != __y.__i_;}
};

FT_END_PRIVATE_NAMESPACE







FT_BEGIN_GLOBAL_NAMESPACE

// ---------------------------------------------------------------
// |                                                             |
// |                   set implementation                        |
// |                                                             |
// ---------------------------------------------------------------
// * map은 LeftLeaningRedBlack class의 adapter class이며, 내부 로직은 모두 __tree__ 멤버 데이터를 통해 이뤄집니다.

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key> >
class FT_TEMPLATE_VIS set
{
public:
    // Member types:
    typedef Key                                         key_type;         // 해당 용량 내 set 요소로 저장된 개체를 정렬 키로 설명하는 형식입니다.
    typedef key_type       	                            value_type;       // 해당 용량 내 set 요소로 저장된 개체를 값으로 설명하는 형식입니다.
    typedef Compare                                     key_compare;      // set의 두 요소간 상대적 순서를 결정하는 두 정렬 키를 비교할 수 있는 함수 개체를 제공하는 형식입니다.
	typedef key_compare									value_compare;	  // 두 요소를 비교하여 set에서 상대적인 순서를 결정할 수 있는 함수 개체를 제공하는 형식입니다.
    typedef Allocator                                   allocator_type;   // set 개체의 allocator 클래스를 나타내는 형식입니다.
    typedef typename allocator_type::reference          reference;        // set에 저장된 요소에 대한 참조를 제공하는 형식입니다.
    typedef typename allocator_type::reference          const_reference;  // 읽기 및 const 작업을 수행하기 위해 const 집합에 저장된 요소에 대한 참조를 제공하는 형식입니다.
    typedef typename allocator_type::pointer            pointer;          // set에서 요소에 대한 포인터를 제공하는 형식입니다.
    typedef typename allocator_type::const_pointer      const_pointer;    // set에서 const 요소에 대한 포인터를 제공하는 형식입니다.
    typedef typename allocator_type::size_type          size_type;        // set에서 요소 수를 표현할 수 있는 부호 없는 정수 형식입니다.
    typedef typename allocator_type::difference_type    difference_type;  // 부호 있는 정수 형식은 반복기가 가리키는 요소 사이의 범위에 있는 set의 요소의 개수를 표현하는 데 사용할 수 있습니다.

private:
    typedef _PRIVATE::LeftLeaningRedBlack<value_type, value_compare, allocator_type>  _set_base;
    typedef FT::set<Key, Compare, Allocator>                  		                  _set_type;
    typedef typename _set_base::node_pointer                                          _node_pointer;
    typedef typename _set_base::node_type                                             _node_type;
    typedef typename _set_base::iterator                                              _node_iterator;
    typedef typename _set_base::const_iterator                                        _node_const_iterator;

 public:
    typedef _PRIVATE::__set_const_iterator<value_type, _node_type>                   iterator; // set에 있는 모든 요소를 읽거나 수정할 수 있는 양방향 반복기를 제공하는 형식입니다.
    typedef _PRIVATE::__set_const_iterator<value_type, _node_type>                   const_iterator; // set에 있는 모든 const 요소를 읽을 수 있는 양방향 반복기를 제공하는 형식입니다.
    typedef typename FT::reverse_iterator<iterator>                                  reverse_iterator; // 역순 set의 요소를 읽거나 수정할 수 있는 양방향 반복기를 제공하는 형식입니다.
    typedef typename FT::reverse_iterator<const_iterator>                            const_reverse_iterator; // set에 있는 모든 const 요소를 읽을 수 있는 양방향 반복기를 제공하는 형식입니다.


private: // Member data:
// * -------------------------------------------------
    _set_base   __tree__;
// * -------------------------------------------------


public: // constructor & destructor.

    FT_HIDE_FROM_ABI
    explicit set(const value_compare& comp = value_compare(), const allocator_type& a = allocator_type())
            : __tree__(comp, a)
    {}

    template <class InputIterator>
    FT_HIDE_FROM_ABI
    set(InputIterator first, InputIterator last, const value_compare& comp = value_compare())
            : __tree__(comp)
    {
        this->insert(first, last);
    }

    template <class InputIterator>
    FT_HIDE_FROM_ABI
    set(InputIterator first, InputIterator last, const value_compare& comp, const allocator_type& a)
            : __tree__(comp, a)
    {
        this->insert(first, last);
    }

    FT_HIDE_FROM_ABI
    set(const _set_type& __s)
            : __tree__(__s.__tree__.value_comp())
    {
		 insert(__s.begin(), __s.end());
	}

    FT_HIDE_FROM_ABI
    explicit set(const allocator_type& a)
            : __tree__(a)
    {}

    FT_HIDE_FROM_ABI
    set(const _set_type& __s, const allocator_type& a)
            : __tree__(__s.__tree__.value_comp(), a)
    {
		 insert(__s.begin(), __s.end());
	}

    ~set()
    { /* destruct everything in tree_base */ }

    FT_HIDE_FROM_ABI
    set& operator=(const _set_type& m)
    {
        __tree__ = m.__tree__;
        return *this;
    }

    FT_HIDE_FROM_ABI
    allocator_type get_allocator() const _NOEXCEPT
    {
        return allocator_type();
    }

public:
    // iterators:
    FT_HIDE_FROM_ABI
    iterator begin() _NOEXCEPT
    { return iterator(__tree__.begin()); }

    FT_HIDE_FROM_ABI
    const_iterator begin() const _NOEXCEPT
    { return const_iterator(__tree__.begin()); }

    FT_HIDE_FROM_ABI
    iterator end() _NOEXCEPT
    { return iterator(__tree__.end()); }

    FT_HIDE_FROM_ABI
    const_iterator end() const _NOEXCEPT
    { return const_iterator(__tree__.end()); }

    FT_HIDE_FROM_ABI
    reverse_iterator rbegin() _NOEXCEPT
    { return reverse_iterator(end()); }

    FT_HIDE_FROM_ABI
    const_reverse_iterator rbegin() const _NOEXCEPT
    { return const_reverse_iterator(end()); }

    FT_HIDE_FROM_ABI
    reverse_iterator rend() _NOEXCEPT
    { return reverse_iterator(begin()); }

    FT_HIDE_FROM_ABI
    const_reverse_iterator rend() const _NOEXCEPT
    { return const_reverse_iterator(begin()); }

public:
    // capacity:
    FT_HIDE_FROM_ABI
    bool empty() const _NOEXCEPT
    { return __tree__.isEmpty(); }

    FT_HIDE_FROM_ABI
    size_type size() const _NOEXCEPT
    { return __tree__.size(); }

    FT_HIDE_FROM_ABI
    size_type max_size() const _NOEXCEPT
    { return __tree__.max_size(); }


public:
    // modifiers:

	// Inserts element(s) into the container, if the container
	// doesn't already contain an element with an equivalent key.
    FT_HIDE_FROM_ABI
    FT::pair<iterator, bool> insert(const value_type& v) _NOEXCEPT
    {
        // check if it has data.
        _node_pointer node_ptr = __tree__.getNode(v);

        if (node_ptr != NULL) // if node already exists.
        {
            return FT::make_pair( iterator( _node_iterator(node_ptr) ), false );
        }
        else // if pair doesn't exist, insert data.
        {
            __tree__.put(v);
            node_ptr = __tree__.getNode(v);
            return FT::make_pair( iterator( _node_iterator(node_ptr) ), true );
        }
    }

    // Inserts value in the position as close as possible to the position just prior to pos
    // The versions with a hint (position) return an iterator pointing to either the newly inserted element
    // or to the element that already had an equivalent key in the set.
    FT_HIDE_FROM_ABI
    iterator insert(const_iterator position, const value_type& v) _NOEXCEPT
    {
        (void)position; // RB tree 구조상 position을 이용하는게 불가능.
        __tree__.put(v);
        _node_pointer node_ptr = __tree__.getNode(v);
        return iterator( typename _set_base::iterator(node_ptr) );
    }

    template <class InputIterator>
    FT_HIDE_FROM_ABI
    void insert(InputIterator first, InputIterator last) _NOEXCEPT
    {
        for (; first != last; ++first) {
            __tree__.put(*first);
        }
    }

    FT_HIDE_FROM_ABI
    void clear() _NOEXCEPT
    {
        __tree__.clear();
    }

    FT_HIDE_FROM_ABI
    void erase( iterator pos ) _NOEXCEPT
    {
        __tree__.erase(*pos);
    }

    // ! ----------------------------------------------------------------------------------
    // ! 삭제 부분에서 트리 재구성으로 인해 iterator가 가리키는 주소가 바뀔 수 있다. itr++ 를 쓰면 안된다.
    FT_HIDE_FROM_ABI
    void erase( iterator first, iterator last ) _NOEXCEPT
    {
        size_type delCount = std::distance(first, last);
        FT::vector<value_type> tmp;
        tmp.reserve(delCount);
        for (iterator itr = first; itr != last; ++itr) {
            tmp.push_back(*itr); // 지울 key 값들을 전부 저장.
        }
        for (size_type i = 0; i < delCount; ++i) {
            __tree__.erase(tmp[i]);
        }
    }
    // ! ----------------------------------------------------------------------------------

    FT_HIDE_FROM_ABI
    size_type erase( const Key& key ) _NOEXCEPT
    {
        return __tree__.erase( key );
    }

    // Exchanges the content of the container by the content of x, which is
    // another map of the same type. Sizes may differ.
    FT_HIDE_FROM_ABI
    void swap( set& other ) _NOEXCEPT
    {
        __tree__.swap(other.__tree__);
    }

public:
    // Lookup:

    //  Returns the number of elements with _key.
    //  This is either 1 or 0 since this container does not allow duplicates.
    FT_HIDE_FROM_ABI
    size_type count( const Key& _key ) const _NOEXCEPT
    {
        if (__tree__.contains( _key ))   return 1;
        else                           		  return 0;
    }

    // Returns an Iterator to an element with key equivalent to _key.
    // If no such element is found, past-the-end (see end()) iterator is returned.
    FT_HIDE_FROM_ABI
    iterator find( const Key& _key ) _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getNode( _key );
        return iterator( _node_iterator(node_ptr) );
    }

    FT_HIDE_FROM_ABI
    const_iterator find( const Key& _key ) const _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getNode( _key );
        return const_iterator( _node_iterator(node_ptr) );
    }

    FT_HIDE_FROM_ABI
    FT::pair<iterator,iterator> equal_range( const Key& _key ) _NOEXCEPT
    {
        return FT::pair<iterator, iterator>(lower_bound(_key), upper_bound(_key));
    }

    FT_HIDE_FROM_ABI
    FT::pair<const_iterator,const_iterator> equal_range( const Key& _key ) const _NOEXCEPT
    {
        return FT::pair<const_iterator, const_iterator>(lower_bound(_key), upper_bound(_key));
    }

    // Returns an iterator pointing to the first element
    // that is not less than (i.e. greater or equal to) key.
    FT_HIDE_FROM_ABI
    iterator lower_bound( const Key& key ) _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getLowerBoundNode( key );
        if (node_ptr == NULL) {
            // If no such element is found, a past-the-end iterator (see end()) is returned.
            return (this->end());
        } else {
            return iterator( _node_iterator(node_ptr) );
        }
    }

    // Returns an iterator pointing to the first element
    // that is greater than key.
    FT_HIDE_FROM_ABI
    const_iterator lower_bound( const Key& key ) const _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getLowerBoundNode( key );
        if (node_ptr == NULL) {
            // If no such element is found, a past-the-end iterator (see end()) is returned.
            return (this->end());
        } else {
            return const_iterator( _node_const_iterator(node_ptr) );
        }
    }

    FT_HIDE_FROM_ABI
    iterator upper_bound( const Key& key ) _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getUpperBoundNode( key );
        if (node_ptr == NULL) {
            // If no such element is found, a past-the-end iterator (see end()) is returned.
            return (this->end());
        } else {
            return iterator( _node_iterator(node_ptr) );
        }
    }

    FT_HIDE_FROM_ABI
    const_iterator upper_bound( const Key& key ) const _NOEXCEPT
    {
        _node_pointer node_ptr = __tree__.getUpperBoundNode( key );
        if (node_ptr == NULL) {
            // If no such element is found, a past-the-end iterator (see end()) is returned.
            return (this->end());
        } else {
            return const_iterator( _node_const_iterator(node_ptr) );
        }
    }

public:
    // Observers:
    FT_HIDE_FROM_ABI
    key_compare key_comp() const _NOEXCEPT
    { return key_compare(); }

    FT_HIDE_FROM_ABI
    value_compare value_comp() const _NOEXCEPT
    { return value_compare(); }

};

// Non-member functions:

// Checks if the contents of lhs and rhs are equal, that is, they have
// the same number of elements and each element in lhs compares
// equal with the element in rhs at the same position.
template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator==(const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return __x.size() == __y.size() && FT::equal(__x.begin(), __x.end(), __y.begin());
}

template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator!=(const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return !(__x == __y);
}


// Compares the contents of lhs and rhs lexicographically.
// The comparison is performed by a function equivalent to
// std::lexicographical_compare. This comparison ignores the map's ordering Compare.
template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator< (const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return FT::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}



template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator> (const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return __y < __x;
}

template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator>=(const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return !(__x < __y);
}

template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
bool
operator<=(const set<Key, Compare, Allocator>& __x,
           const set<Key, Compare, Allocator>& __y)
{
    return !(__y < __x);
}

// specialized algorithms:
template <class Key, class Compare, class Allocator>
inline FT_HIDE_FROM_ABI
void
swap(set<Key, Compare, Allocator>& __x, set<Key, Compare, Allocator>& __y) /* (C++17) _NOEXCEPT_(_NOEXCEPT_(x.swap(y))) */
{
    __x.swap(__y);
}

FT_END_GLOBAL_NAMESPACE



#endif //FT_CONTAINER_SET_HPP
