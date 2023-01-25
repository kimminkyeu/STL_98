//
// Created by 김민규 on 2023/01/20.
//

#ifndef FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
#define FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP


// TODO:  remove later
#include <iostream>
#include <cstdlib>


#include <exception>    // for exception
#include <functional>   // for std::less
#include "__config.hpp" // for macros
#include "utility.hpp"  // for ft::pair

FT_BEGIN_PRIVATE_NAMESPACE



// ---------------------------------------------------------------
// |                                                             |
// |             Binary Tree iterator implementation             |
// |                                                             |
// ---------------------------------------------------------------

// forward declaration
template <typename _Key, typename _NodeType>
class __tree_const_iterator;



template <typename _Key, typename _NodeType>
class FT_TEMPLATE_VIS __tree_iterator
 : public std::iterator< std::bidirectional_iterator_tag, _NodeType >
{
private: // typedef from class Left_Leaning_Red_Black
    typedef std::iterator< std::bidirectional_iterator_tag, _NodeType >		_Iter_base;
    typedef __tree_const_iterator<_Key, _NodeType>                          _const_iterator_type;
    typedef __tree_iterator<_Key, _NodeType>                                _iterator_type;

public:
    typedef typename _Iter_base::iterator_category            iterator_category;   // bidirectional tag
    typedef typename _Iter_base::value_type           		  value_type; 		   // (NodeType)
    typedef value_type                                        node_type;
    typedef typename _Iter_base::difference_type              difference_type;     // ptrdiff_t

    // * std::iterator 에는 const_type이 정의되어 있지 않으므로, 여기서는 직접 정의하였다.
    typedef _NodeType*                                        pointer;
    typedef const _NodeType*                                  const_pointer;
    typedef _NodeType&                                        reference;
    typedef const _NodeType&                                  const_reference;


protected: // data member
// * --------------------------------------------------------------------
    pointer m_Current; // pointer to a single node.
    pointer m_LastNode;    // pointer to the last node of tree. --> --end() 할 때 필요함.
// * --------------------------------------------------------------------

public: // constructor & destructor

    pointer base() const
    { return m_Current; }

    __tree_iterator()
            : m_Current(NULL)
    {}

    // constructor Wrapping
    __tree_iterator(const __tree_iterator& _iterator_in)
            : m_Current(_iterator_in.base()), m_LastNode(_iterator_in.m_LastNode)
    {}


    // * Node pointer to iterator casting.
    // end()를 호출할 경우에만 두번째 파라미터에 last_node의 정보가 들어온다.
    // end()를 호출한게 아니라면, 두번재 파라미터는 NULL이 된다.
    __tree_iterator(const pointer& _node_pointer_in, const pointer& _last_node_hint = NULL)
        : m_Current(_node_pointer_in), m_LastNode(_last_node_hint)
    {}


    // 다른 key type을 가리키는 iterator간의 casting. (ex. 서로 다른 type을 node의 KEY로 가진 tree)
    template<typename _Other_Key>
    __tree_iterator(const __tree_iterator<_Other_Key, _NodeType>& _non_const_iterator)
        : m_Current(_non_const_iterator.base()), m_LastNode(_non_const_iterator.m_LastNode)
    {}



       // * iterator를 const_iterator로 캐스팅 할 경우에 사용된다.
    template<typename Key, typename NodeType>
    __tree_iterator(const __tree_const_iterator<Key, NodeType>& _iterator_in)
            : m_Current(_iterator_in.base()), m_LastNode(_iterator_in.m_LastNode)
    {}


public: // operator:

    reference operator*() const // this returns (node&)
    { return (*m_Current); }

    pointer operator->() const // this returns (node pointer->)
    { return (m_Current); }

     template<typename _Other_Key>
    _iterator_type& operator=(const __tree_iterator<_Other_Key, _NodeType>& _other_iterator)
    {
        m_Current = _other_iterator.base();
        return (*this);
    }

    _iterator_type operator++(int) // same as iter++;
    {
        pointer tmp = m_Current;
        if (m_Current != NULL) { // if NULL, then do nothing.
            m_Current = m_Current->getSuccessor();
        }
        return __tree_iterator(tmp);
    }

    _iterator_type& operator++() // same as ++iter;
    {
        if (m_Current != NULL) { // if NULL, then do nothing.
            m_Current = m_Current->getSuccessor();
        }
        return *this;
    }

    _iterator_type operator--(int) // same as iter--;
    {
        pointer tmp = m_Current;
        if (m_Current != NULL) { // if NULL, then do nothing.
            m_Current = m_Current->getPredecessor();
        }
        return __tree_iterator(tmp);
    }

    // ! end()와 NULL을 어떻게 구분할 것인가?
    // * end()일 경우에만 iterator에 m_last_node 정보가 들어온다.
    // * 이를 통해 --iterator(NULL)과 --iterator(end())를 구분할 수 있다.
    _iterator_type& operator--() // same as --iter;
    {
        if (m_Current != NULL) {
            m_Current = m_Current->getPredecessor();
        } else if (m_LastNode != NULL) { // if end(), return last element.
            m_Current = m_LastNode;
        }
        return *this;
    }

    // * ---------------------------------------------------------
    // * tree_iterator에서 tree_const_iterator로 캐스팅할 때
    // * 복사 생성자 부분의 base를 사용하기 위해 friend로 선언하였습니다.
    template<typename, typename>
    FT_HIDE_FROM_ABI friend class __tree_const_iterator;
    // * ---------------------------------------------------------
};

// @ forward iterator requirements.
// left-hand-side / right_hand_side

template<typename _IteratorL, typename _IteratorR, typename _NodeType>
bool operator==(const __tree_iterator<_IteratorL, _NodeType>& __lhs,
                const __tree_iterator<_IteratorR, _NodeType>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _NodeType>
bool operator!=(const __tree_iterator<_IteratorL, _NodeType>& __lhs,
                const __tree_iterator<_IteratorR, _NodeType>& __rhs)
{ return !(__lhs == __rhs); }




// ---------------------------------------------------------------
// |                                                             |
// |         Binary Tree const iterator implementation           |
// |                                                             |
// ---------------------------------------------------------------
template <typename _Key, typename _NodeType>
class FT_TEMPLATE_VIS __tree_const_iterator
 : public std::iterator< std::bidirectional_iterator_tag, _NodeType >
{
private: // typedef from class Left_Leaning_Red_Black
    typedef std::iterator< std::bidirectional_iterator_tag, _NodeType >		_Iter_base;
    typedef __tree_const_iterator<_Key, _NodeType>                          _const_iterator_type;
    typedef __tree_iterator<_Key, _NodeType>                                _none_const_iterator_type;


public:
    typedef typename _Iter_base::iterator_category            iterator_category;   // bidirectional tag
    typedef typename _Iter_base::value_type           		  value_type; 		   // (NodeType)
    typedef value_type                                        node_type;
    typedef typename _Iter_base::difference_type              difference_type;     // ptrdiff_t

    // * std::iterator 에는 const_type이 정의되어 있지 않으므로, 여기서는 직접 정의하였다.
    typedef _NodeType*                                        pointer;
    typedef const _NodeType*                                  const_pointer;
    typedef _NodeType&                                        reference;
    typedef const _NodeType&                                  const_reference;

protected: // data member
// * --------------------------------------------------------------------
    const_pointer m_Current;     // const node pointer
    const_pointer m_LastNode;    // const node pointer to the last node of tree. --> --end() 할 때 필요함.
// * --------------------------------------------------------------------

public: // constructor & destructor

    const_pointer base() const
    { return m_Current; }

    __tree_const_iterator(void)
            : m_Current(NULL), m_LastNode(NULL)
    {}

    // * iterator를 const_iterator로 캐스팅 할 경우에 사용된다.
    template<typename Key, typename NodeType>
    __tree_const_iterator(const __tree_iterator<Key, NodeType>& _iterator_in)
            : m_Current(_iterator_in.base()), m_LastNode(_iterator_in.m_LastNode)
    {}

    // * Node pointer to iterator casting.
    // end()를 호출할 경우에만 두번째 파라미터에 last_node의 정보가 들어온다.
    // end()를 호출한게 아니라면, 두번재 파라미터는 NULL이 된다.
    explicit __tree_const_iterator(const pointer& _node_pointer_in, const pointer& _last_node_hint = NULL)
            : m_Current(_node_pointer_in), m_LastNode(_last_node_hint)
    {}

    // constructor Wrapping
    explicit __tree_const_iterator(const __tree_const_iterator& _const_iterator_in)
            : m_Current(_const_iterator_in.base()), m_LastNode(_const_iterator_in.m_LastNode)
    {}

    // 다른 key type을 가리키는 iterator간의 casting. (ex. 서로 다른 type을 node의 KEY로 가진 tree)
    template<typename _Other_Key>
    __tree_const_iterator(const __tree_const_iterator<_Other_Key, _NodeType>& _other_iterator)
            : m_Current(_other_iterator.base()), m_LastNode(_other_iterator.m_LastNode)
    {}

public: // operator:

    const_reference operator*() const // this returns node's Key.
    { return (*m_Current); }

    const_pointer operator->() const // this makes [ Iter->... ] equal to [ Key ->... ]
    { return (m_Current); }

     template<typename _Other_Key>
     __tree_const_iterator& operator=(const __tree_const_iterator<_Other_Key, _NodeType>& _other_iterator)
    {
        m_Current = _other_iterator.base();
        return (*this);
    }

    _const_iterator_type operator++(int) // same as iter++;
    {
        const_pointer tmp = m_Current;
        if (m_Current != NULL) {
            m_Current = m_Current->getSuccessor();
        }
        return __tree_const_iterator(tmp);
    }

    _const_iterator_type& operator++() // same as ++iter;
    {
        if (m_Current != NULL) {
            m_Current = m_Current->getSuccessor();
        }
        return *this;
    }

    _const_iterator_type operator--(int) // same as iter--;
    {
        pointer tmp = m_Current;
        if (m_Current != NULL) { // if NULL, then do nothing.
            m_Current = m_Current->getPredecessor();
        }
        return __tree_const_iterator(tmp);
    }

    // ! end()와 NULL을 어떻게 구분할 것인가?
    // * end()일 경우에만 iterator에 m_last_node 정보가 들어온다.
    // * 이를 통해 --iterator(NULL)과 --iterator(end())를 구분할 수 있다.
    _const_iterator_type& operator--() // same as --iter;
    {
        if (m_Current != NULL) {
            m_Current = m_Current->getPredecessor();
        } else if (m_LastNode != NULL) { // if end(), return last element.
            m_Current = m_LastNode;
        }
        return *this;
    }
};

// @ forward iterator requirements.
// left-hand-side / right_hand_side

template<typename _IteratorL, typename _IteratorR, typename _NodeType>
bool operator==(const __tree_const_iterator<_IteratorL, _NodeType>& __lhs,
                const __tree_const_iterator<_IteratorR, _NodeType>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _NodeType>
bool operator!=(const __tree_const_iterator<_IteratorL, _NodeType>& __lhs,
                const __tree_const_iterator<_IteratorR, _NodeType>& __rhs)
{ return !(__lhs == __rhs); }












// ---------------------------------------------------------------
// |                                                             |
// |             Tree_node_alloc_base implementation             |
// |                                                             |
// ---------------------------------------------------------------
// wrapper class to handle all the node allocating, rebind, etc...
// 주어진 T를 각 tree에 맞는 node로 묶어서 하나의 단위로 allocate.

template <class _KeyType, class _NodeType, class Allocator>
class Tree_node_alloc_base
{
protected: // typedef and namespace scope

    // @ rebinding allocator<T> to allocator<node<T>>
    // _NodeType is <RbNode<T, Compare>.
    typedef typename Allocator::template rebind<_NodeType>::other  node_allocator_type;

    // @ using typename of rebound allocator's typedef
    typedef _NodeType                                           node_type;
    typedef _KeyType                                            key_type;
    typedef key_type*                                           key_pointer;

    typedef typename node_allocator_type::size_type		        size_type;
    typedef typename node_allocator_type::pointer		        pointer;
    typedef typename node_allocator_type::const_pointer		    const_pointer;
    typedef typename node_allocator_type::reference		        reference;
    typedef typename node_allocator_type::const_reference		const_reference;
    typedef typename node_allocator_type::difference_type		difference_type;


protected:
    // * ------------------------------------------------------
    node_allocator_type __m_Data_allocator;
    // * ------------------------------------------------------

    FT_HIDE_FROM_ABI // allocate single node. same as ::new node();
    pointer __allocate_single_node()
    {
        return __m_Data_allocator.allocate(sizeof(node_type));
    }

    FT_HIDE_FROM_ABI
    void __deallocate_single_node(pointer _address_of_node)
    {
        if (_address_of_node != NULL) {
            __m_Data_allocator.deallocate(_address_of_node, sizeof(node_type));
        }
    }

    FT_HIDE_FROM_ABI
    void __construct_node_at(pointer _address_of_node) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_type();
    }

    FT_HIDE_FROM_ABI
     void __construct_node_by_value_at(pointer _address_of_node, const node_type& _value) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_type(_value);
    }

    FT_HIDE_FROM_ABI
    void __destroy_node_at(pointer _address_of_node) {
        _address_of_node->node_type::~node_type(); // directly call destructor.
    }

// ! -------------------------------------------------------------------------------------
// ! [ 매우 중요한 함수들 ]
// !    - map은 key가 pair<const T, U> 이기 때문에, 이미 만들어진 key에 값을 복사할 수 없다.
// !    - 따라서 일괄적으로 key를 소멸, 재생성해야 한다.
// !                   Ex) [ node_ptr->key = other_key ] 는 ft::map에서 컴파일되지 않는다.

    FT_HIDE_FROM_ABI
    void __construct_key_by_value_at(key_pointer _address_of_key, const key_type& _key) {
        new (_address_of_key) key_type(_key); // call constructor
    }

    FT_HIDE_FROM_ABI
    void __destroy_key_at(key_pointer _address_of_key) {
        _address_of_key->key_type::~key_type(); // call destructor
    }
// ! --------------------------------------------------------------------------------------


protected:
    Tree_node_alloc_base()
        : __m_Data_allocator(node_allocator_type())
    {}

    explicit Tree_node_alloc_base(const Allocator& _a)
            : __m_Data_allocator(node_allocator_type(_a)) // _a를 받아서, node_allocator_type으로 바꿈.
    {}
};

















// --------------------------------------------------------------------------
// |                                                                        |
// |          Red Black tree Node implementation                            |
// |                                                                        |
// --------------------------------------------------------------------------

const bool RED = false;
const bool BLACK = true;

// Node for Red Black tree
// passed template parameter _Compare for __key_compare member function.
template <typename _KeyType, class _Compare>
struct RedBlackNode
{
    // (1) typedef for node_type
    typedef RedBlackNode<_KeyType, _Compare>     node_type;
    typedef node_type*                           node_pointer;
    typedef const node_type*                     const_node_pointer;

    // (2) typedef for key_type
    typedef _KeyType                             key_type;
    typedef _KeyType*                            key_type_pointer;
    typedef const _KeyType*                      const_key_type_pointer;    // for const_map_iterator
    typedef _KeyType&                            key_type_reference;
    typedef const _KeyType&                      const_key_type_reference;  // for const_map_iterator
    typedef _Compare                             value_compare;

// *-- member data ----------------------------------------------------------------------------
    _KeyType            key;  // key would be std::pair<A, B> if map. if set, key would be single type.
    bool                color;          // color of parent link
    RedBlackNode*       left;
    RedBlackNode*       right;
    RedBlackNode*       parent;         // for iterator system
// *-------------------------------------------------------------------------------------------

    explicit RedBlackNode(_KeyType _key) _NOEXCEPT
            : key(_key), left(NULL), right(NULL), color(RED), /* size(0),*/ parent(NULL)
    {};

    RedBlackNode(_KeyType _key, bool _color) _NOEXCEPT
            : key(_key), color(_color), left(NULL), right(NULL), parent(NULL)
    {}

    RedBlackNode(const node_type& other) _NOEXCEPT
            : key(other.key), color(other.color), left(other.left), right(other.right), parent(other.parent)
    {}

    // if Successor doesn't exist, then return NULL.
    node_pointer getSuccessor() const _NOEXCEPT
    {
        // Case 1.
        if (this->right != NULL)
        {
            node_pointer curr = this->right;
            while (curr->left != NULL) {
                curr = curr->left;
            }
            return curr;
        }
        // Case 2.
        node_pointer above = this->parent;
        const_node_pointer me = this;
        while (above != NULL && me == above->right) // go up if I'm right node of parent.
        {
            me = above;
            above = above->parent;
        }
        return above;
    }

    // if Predecessor doesn't exist, then return NULL.
    node_pointer getPredecessor() const _NOEXCEPT
    {
        // Case 1.
        if (this->left != NULL)
        {
            node_pointer curr = this->left;
            while (curr->right != NULL) {
                curr = curr->right;
            }
            return curr;
        }
        // Case 2.
        node_pointer above = this->parent;
        const_node_pointer me = this;
        while (above != NULL && me == above->left) // go up if I'm left node of parent.
        {
            me = above;
            above = above->parent;
        }
        return above;
    }
};















// ---------------------------------------------------------------
// |                                                             |
// |        Left Leaning Red Black Tree implementation           |
// |                                                             |
// ---------------------------------------------------------------
// *Ref 0: [ Original paper of Sedgewick ]
//        https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
// *Ref 1: [ Sedgewick's lecture (1) ]
//        https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/RedBlackBST.html
// *Ref 1: [ Sedgewick's lecture (2) ]
//        https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/RedBlackBST.java.html
// *Ref 3: [ Other lecture reference ]
//        https://cs.pomona.edu/classes/cs062-2019fa/lectures/Lecture28_29.pdf
// *Ref 4: [ C++ source lecture on LLRB ]
//        http://www.teachsolaisgames.com/articles/balanced_left_leaning.html




// if Map, KeyType will be ft::pair<A, B>.  if Set, KeyType will be A.
template <class _KeyType, class _Compare = std::less<_KeyType>, class _Allocator = std::allocator<_KeyType> >
class LeftLeaningRedBlack : protected Tree_node_alloc_base<_KeyType, // * Rebind는 alloc_base에서 담당.
                                                           RedBlackNode<_KeyType, _Compare>,
                                                           _Allocator>
{

public: // typedefs
    typedef RedBlackNode<_KeyType, _Compare>                            node_type;
    typedef _KeyType                                                    key_type;
    typedef _Compare                                                    value_compare_type;
    typedef _Allocator                                                  allocator_type;
    typedef LeftLeaningRedBlack<key_type, _Compare, _Allocator>         tree_type;

protected:
    typedef Tree_node_alloc_base<key_type, node_type, _Allocator>       _node_alloc_base;
    typedef typename _node_alloc_base::node_allocator_type              _node_allocator_type;

public:
    typedef typename _node_allocator_type::difference_type               difference_type;
    typedef typename _node_allocator_type::size_type                     size_type;
    typedef typename _node_allocator_type::pointer                       node_pointer;

    typedef __tree_iterator<key_type, node_type>                         iterator;
    typedef __tree_const_iterator<key_type, node_type>                   const_iterator;


private: // private data member
// *-----------------------------------------------------------------------------
    node_pointer    m_Root;
    _Compare        m_Value_compare;    // default compare function class.
// *-----------------------------------------------------------------------------


public: // constructor, destructor

    // Initializes an empty symbol table.
    explicit LeftLeaningRedBlack(const value_compare_type& comp = value_compare_type(), const _Allocator& alloc = allocator_type())
        : _node_alloc_base(alloc), m_Root(NULL), m_Value_compare(comp)
    {}

    LeftLeaningRedBlack(const tree_type& other_tree)
        : m_Value_compare(other_tree.m_Value_compare)
    {
        put(other_tree.begin(), other_tree.end());
    }

    explicit LeftLeaningRedBlack(const allocator_type& a)
        : _node_alloc_base(a), m_Root(NULL), m_Value_compare(value_compare_type())
    {}

    LeftLeaningRedBlack(const tree_type& other_tree, const allocator_type& a)
        : _node_alloc_base(a), m_Value_compare(other_tree.m_Value_compare)
    {
        put(other_tree.begin(), other_tree.end());
    }

    template <class InputIterator>
    LeftLeaningRedBlack(InputIterator first, InputIterator last, const value_compare_type& comp = value_compare_type())
        : m_Root(NULL), m_Value_compare(comp)
    {
        put(first, last);
    }

    template <class InputIterator>
    LeftLeaningRedBlack(InputIterator first, InputIterator last, const value_compare_type& comp, const allocator_type& a)
        : _node_alloc_base(a), m_Root(NULL), m_Value_compare(comp)
    {
        put(first, last);
    }

    ~LeftLeaningRedBlack()
    {
        // delete tree every data.
        clear();
        m_Root = NULL;
    }

    // contents 깊은 복사해야지 포인터를 다룰게 아니다.
    tree_type& operator=(const tree_type& other_tree)
    {
        // (1) clear tree.
        clear();
        // (2) copy data
        put(other_tree.begin(), other_tree.end());
        return *this;
    }


// ------------------------------------------------------------
//       RB Node helper method.
// ------------------------------------------------------------

private:
    node_pointer __allocate_and_construct_RbNode(const key_type& _key, const bool& _color)
    {
        node_pointer address_of_new_node = this->__allocate_single_node();
        this->__construct_node_by_value_at(address_of_new_node, node_type(_key, _color));
        return address_of_new_node;
    }

    // ! Re-construct key_type object with given value.
    // ! 이 함수는 key_type을 pair<const T, U> 로 사용하는 ft::map 을 위하여 추가하였습니다.
    // ! 이미 존재하는 key instance에 다른 key의 값을 대입할 수 없기 때문입니다.
    void __reconstruct_key_by_value_at(key_type* address_of_key_to_change, const key_type& other_key)
    {
        this->__destroy_key_at(address_of_key_to_change);
        this->__construct_key_by_value_at(address_of_key_to_change, other_key);
    }

    void __destroy_and_deallocate_RbNode(node_pointer p)
    {
        this->__destroy_node_at(p);
        this->__deallocate_single_node(p);
    }

    int __compare_value(const key_type &lhs, const key_type &rhs) const _NOEXCEPT
    {
        if (m_Value_compare(lhs, rhs) == true) { // lhs < rhs
            return 1;
        } else if (m_Value_compare(rhs, lhs) == true) { // lhs > rhs
            return -1;
        }
        return 0; // this == other
    }

    // color is black if node is null.
    bool __isRed(node_pointer x) const _NOEXCEPT
    {
        if (x == NULL) return false; // null node is black.
        return (x->color == RED);
    }

// ------------------------------------------------------------
//       Standard BST search
// ------------------------------------------------------------
public:

    iterator begin() _NOEXCEPT
    {
        node_pointer t = __min(m_Root);
        return iterator(t);
    }

    const_iterator begin() const _NOEXCEPT
    {
        node_pointer t = __min(m_Root);
        return const_iterator(t);
    }

    // this returns address of last node. (=Largest node)
    node_pointer getLastNode() const _NOEXCEPT
    {
        node_pointer t = __max(m_Root);
        return t;
    }

    // * -----------------------------------------------------------------
    // * iterator(NULL)과 iterator(end())를 구분하기 위함.
    // * --end()는 마지막 노드를 반환하기 떄문.
    iterator end() _NOEXCEPT
    { return iterator(NULL, getLastNode()); }

    const_iterator end() const _NOEXCEPT
    { return const_iterator(NULL, getLastNode()); }
    // * -----------------------------------------------------------------

    // return true is tree is empty
    bool isEmpty() const _NOEXCEPT
    {
        return (m_Root == NULL);
    }

    // find node, which contains target key.
    node_pointer getNode(const key_type& target_key) const _NOEXCEPT
    {
        return __getNode(m_Root, target_key);
    }

    // if key exist, then return pointer to the key
    key_type*  getAddressOfKey(const key_type& target_key) const _NOEXCEPT
    {
        node_pointer ptr = getNode(target_key);

        if (ptr == NULL)    return NULL; // key not found.
        else                return &(ptr->key);
    }

    // target_key 보다 크거나 같은 값.
    node_pointer getLowerBoundNode(const key_type& target_key) const _NOEXCEPT
    {
        if (m_Root == NULL) {
            return NULL;
        }
        node_pointer nearNode = __getClosestNode(m_Root, target_key);
        const int cmp = __compare_value(nearNode->key, target_key);

        if (cmp <= 0) { // lhs >= rhs, 즉 nearNode 가 targetKey 보다 크거나 같다.
            return nearNode;
        } else { // lhs < rhs, 즉 nearNode 가 targetKey 보다 작다.
            return nearNode->getSuccessor();
        }
    }

    // target_key 보다 큰 값 중 가장 가까운 값
    node_pointer getUpperBoundNode(const key_type& target_key) const _NOEXCEPT
    {
        if (m_Root == NULL) {
            return NULL;
        }
        node_pointer nearNode = __getClosestNode(m_Root, target_key);
        const int cmp = __compare_value(nearNode->key, target_key);

        if (cmp < 0) { // lhs > rhs, 즉 nearNode 가 targetKey 보다 크다.
            return nearNode;
        } else { // lhs <= rhs, 즉 nearNode 가 targetKey 보다 작거나 같다.
            return nearNode->getSuccessor();
        }
    }


private:
    // 찾고자 하는 key 값에 근접한 노드의 주소 반환. 다만 key보다 큰지 작은지는 추가 검사 필요.
    node_pointer __getClosestNode(node_pointer curr, const key_type& target_key) const _NOEXCEPT
    {
        while (curr != NULL)
        {
            int cmp = __compare_value(curr->key, target_key);

            if        ((cmp < 0) && (curr->left != NULL))    curr = curr->left;
            else if   ((cmp > 0) && (curr->right != NULL))   curr = curr->right;
            else       break;
        }
        return curr;
    }

    // because std::less only returns true or false, we need to handle equal value.
    node_pointer __getNode(node_pointer curr, const key_type& target_key) const _NOEXCEPT
    {
        while (curr != NULL)
        {
            const int cmp = __compare_value(curr->key, target_key);

            if      (cmp < 0)   curr = curr->left;
            else if (cmp > 0)   curr = curr->right;
            else                return curr;
        }
        return NULL; // key not found
    }

// ------------------------------------------------------------
//       LLRBT helper method.
// ------------------------------------------------------------

public:
    size_type size() const _NOEXCEPT
    {
        // ! 바로 여기서 문제 발생. 일반 iterator를 const_iterator로 생성할 때 발생함.
        // const_iterator i = const_iterator(begin());
        // const_iterator j = const_iterator(end());
        // return (std::distance(i, j));
        return (std::distance(begin(), end()));
    }

    size_type max_size() const _NOEXCEPT
    {
        return (std::numeric_limits<size_type>::max() / sizeof(node_type));
    }

    // swap data with another red black tree
    void swap(tree_type& other) _NOEXCEPT
    {
        FT::swap(this->m_Root, other.m_Root);
        FT::swap(this->m_Value_compare, other.m_Value_compare);
    }


    // Does this symbol table contain the given key?
    bool contains(const key_type& key) const _NOEXCEPT
    {
        return (getNode(key) != NULL);
    }

    // ------------------------------------------------------------
    //       Red Black tree insertion
    // ------------------------------------------------------------
public:
    void put(const key_type& key)
    {
        m_Root = __put(m_Root, key);
        m_Root->color = BLACK; // root color must be black.
    }

    // insert unique data to red black tree.
    // void put(node_pointer start, node_pointer end)
    template <class InputIterator>
    void put(InputIterator start, InputIterator end)
    {
        while (start != end)
        {
            put(start->key);
            ++start;
        }
    }

private:

    node_pointer __put(node_pointer curr, const key_type& target_key)
    {
        // if end node.
        if (curr == NULL) {
             return (__allocate_and_construct_RbNode(target_key, RED));
        }
        // if not empty tree, then recursively insert data.
        // const int cmp = curr->compare_key_to(target_key);
        const int cmp = __compare_value(curr->key, target_key);

        if      (cmp < 0)   {
            curr->left  = __put(curr->left, target_key);
            curr->left->parent = curr;
        }
        else if (cmp > 0)   {
            curr->right = __put(curr->right, target_key);
            curr->right->parent = curr;
        }
        else                {
            // if key exist, the element is not inserted
        }

        // ----------------------------------------
        // |    Fix-up any right-leaning links    |
        // ----------------------------------------

        // * Case 1. right leaning red
        /*
              Curr 0
                 / |
                /  |
               0   O -> Red                                 */
        if (__isRed(curr->right) && !__isRed(curr->left)) { curr = __rotateLeft(curr); }

        // * Case 2. double left red
        // recursive call of Case 1 and 2 handles all the (p->left) && (p->left->right) red node.
        /*
              Curr 0
                  /    -->   if left and left's left is red
            Red O
               /
          Red O                                              */
        if (__isRed(curr->left) && __isRed(curr->left->left)) { curr = __rotateRight(curr); }

        // * Case 3. two red child
        /*
                   Curr 0
                      / |
                     /  |
             Red <- O   O -> Red                             */
        if (__isRed(curr->right) && __isRed(curr->left)) { __flipColors(curr); }

        return curr;
    }

    // ------------------------------------------------------------
    //       Red Black tree deletion
    // ------------------------------------------------------------
    // http://www.teachsolaisgames.com/articles/balanced_left_leaning.html
    // 여기를 보면서 상세한 내용을 이해하는게 좋겠다.

public:
    // Removes the smallest key and associated value from the symbol table.
    void eraseMin()
    {
        if (isEmpty()) throw std::runtime_error("BST underflow");

        // if both children of root are black, set root to red.
        if (!__isRed(m_Root->left) && !__isRed(m_Root->right)) {
            m_Root->color = RED;
        }

        m_Root = __eraseMin(m_Root);
        if (!isEmpty()) {
            m_Root->color = BLACK;
        }
    }

private:

    // ? 이 함수가 호출되는 순간은, 삭제하려는 노드의 successor를 복사한 이후이다.
    // ? 이 함수의 파라미터에는 삭제노드의 오른쪽 노드가 들어온다.
    // ? 삭제노드의 오른쪽 노드의 가장 왼쪽 값(후속자 노드)를 찾아서 제거한다.
    // After replacing the deleted value with a different key,
    // the empty leaf node selected by FindMin can be deleted by calling _eraseMin()
    // This replacement key is found with __min function
    node_pointer __eraseMin(node_pointer curr)
    {
        // * ----------------------------------------------------
        if (curr->left == NULL) // ! 똑같이, 여기서 삭제 발생.
        {
            __destroy_and_deallocate_RbNode(curr);
            return NULL;
        }
        // * ----------------------------------------------------

        // If these nodes are black, we need to rearrange this subtree to
        // force the left child to be red.
        if (!__isRed(curr->left) && !__isRed(curr->right))
        {
            curr = __moveRedLeft(curr);
        }

        // Continue recursing to locate the node to delete.
        curr->left = __eraseMin(curr->left);

        // Fix right-leaning red nodes and eliminate 4-nodes on the way up.
        // Need to avoid allowing search operations to terminate on 4-nodes,
        // or searching may not locate intended key.
        return __fixUp(curr);
    }

public:
    // Remove everything.
    void clear()
    {
        iterator itr = begin();
        while (itr != end())
        {
            ++itr;
            eraseMin();
        }
    }

    // Removes the specified key and its associated value from this symbol table
    // Returns number of elements erased (0 or 1)
    size_type erase(const key_type& key)
    {
        if (!contains(key)) { return 0; }

        m_Root = __erase(m_Root, key);

        // Assuming we have not deleted the last node from the tree, we
        // need to force the root to be a black node to conform with the rules of a red-black tree.
        if (!isEmpty()) { m_Root->color = BLACK; }
        return 1;
    }

private:
    node_pointer __erase(node_pointer curr, const key_type& key)
    {
        if (__compare_value(curr->key, key) < 0)
        {
            // If pNode and pNode->pLeft are black, we may need to
            // move pRight to become the left child if a deletion
            // would produce a red node.
            if (!__isRed(curr->left) && !__isRed(curr->left->left)) {
                curr = __moveRedLeft(curr);
            }
            // 다시 왼쪽 기준 삭제 호출.
            curr->left = __erase(curr->left, key);
        }
        else
        {
            // If the left child is red, apply a rotation so we make
            // the right child red.
            if (__isRed(curr->left)) {
                curr = __rotateRight(curr);
            }

            // Special case for deletion of a leaf node.
            // The arrangement logic of LLRBs assures that in this case,
            // pNode(curr) cannot have a left child.
            if (__compare_value(curr->key, key) == 0 // if leaf
                && (curr->right == NULL))
            {
                __destroy_and_deallocate_RbNode(curr);
                return NULL;
            }

            // If we get here, we need to traverse down the right node.
            // However, if there is no right node, then the target key is
            // not in the tree, so we can break out of the recursion.
            if (curr->right != NULL)
            {
                if (!__isRed(curr->right) && !__isRed(curr->right->left)) {
                    curr = __moveRedRight(curr);
                }

                // Deletion of an internal node: We cannot delete this node
                // from the tree, so we have to find the node containing
                // the smallest key value that is larger than the key we're
                // deleting.  This other key will replace the value we're
                // deleting, then we can delete the node that previously
                // held the key/value pair we just moved.
                if (__compare_value(curr->key, key) == 0) // if found key
                {
                    // curr->key = (__min(curr->right))->key; --> 왼쪽 코드는 map에서 컴파일 되지 않습니다.
                    // 따라서 아래와 같이 key만 재생성하는 함수를 사용하였습니다.
                    // (1) copy successor to current key
                    const key_type successor_key = (__min(curr->right))->key;
                    __reconstruct_key_by_value_at(&(curr->key), successor_key);
                    // (2) delete successor node.
                    curr->right = __eraseMin(curr->right);
                }
                else
                {
                    curr->right = __erase(curr->right, key);
                }
                // -----------------------------------------------
            }
        }

        // Fix right-leaning red nodes and eliminate 4-nodes on the way up.
        // Need to avoid allowing search operations to terminate on 4-nodes,
        // or searching may not locate intended key.
        return __fixUp(curr);
    }







private:
    // ------------------------------------------------------------
    //      * Red Black tree elementary operations
    // ------------------------------------------------------------


    // flip the colors of a node and its two children
    void __flipColors(node_pointer h) _NOEXCEPT
    {
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

    // make a right-leaning link lean to the left
    node_pointer __rotateLeft(node_pointer h) _NOEXCEPT
    {
        node_pointer new_parent = h->right;
        h->right = new_parent->left;

        // + update parent pointer for iterators
        if (h->right != NULL) { h->right->parent = h; }

        new_parent->left = h;
        new_parent->color = h->color;
        h->color = RED;

        // + update parent pointer for iterators
        node_pointer original_grandparent = h->parent;
        h->parent = new_parent;
        new_parent->parent = original_grandparent;

        return new_parent;
    }

    // make a left-leaning link lean to the right
    node_pointer __rotateRight(node_pointer h) _NOEXCEPT
    {
        node_pointer new_parent = h->left;
        h->left = new_parent->right;

        // + update parent pointer for iterators
        if (h->left != NULL) { h->left->parent = h; }

        new_parent->right = h;
        new_parent->color = h->color;
        h->color = RED;

        // + update parent pointer for iterators
        node_pointer original_grandparent = h->parent;
        h->parent = new_parent;
        new_parent->parent = original_grandparent;

        return new_parent;
    }


    // Assuming that h is red and both h.left and h.left.left are black,
    // make h.left or one of its children red.
    // Ref 1 : http://www.teachsolaisgames.com/articles/balanced_left_leaning.html
    // Ref 2 : https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
    node_pointer __moveRedLeft(node_pointer h) _NOEXCEPT
    {
        // If both children are black, we turn these three nodes into a
        // Temporary 4-node by applying a color dlip
        __flipColors(h);

        // But we may end up with a case where h->Right has a red child.
        // Apply a pair of rotations and a color flip to make pNode a
        // red node, both of its children become black nodes, and pLeft
        // becomes a 3-node.
        if (h->right != NULL && __isRed(h->right->left))
        {
            h->right = __rotateRight(h->right);
            h = __rotateLeft(h);

            __flipColors(h);
        }
        return h;
    }

    // Assuming that h is red and both h.right and h.right.left
    // are black, make h.right or one of its children red.
    node_pointer __moveRedRight(node_pointer h)
    {
        // Applying a color flip may turn pNode into a 4-node,
        // with both of its children being red.
        __flipColors(h);

        // However, this may cause a situation where both of pNode's
        // children are red, along with pNode->pLeft->pLeft.  Applying a
        // rotation and a color flip will fix this special case, since
        // it makes pNode red and pNode's children black.
        if (h->left != NULL && __isRed(h->left->left))
        {
            h = __rotateRight(h);
            __flipColors(h);
        }
        return h;
    }

    // Restore red-black tree invariant. ( * Same Logic we used in __put function )
    // As we recurse down the tree, the code will leave right-leaning red nodes and unbalanced 4-nodes.
    // These rule violations will be repaired when recursing back out of the tree by the FixUp function.
    node_pointer __fixUp(node_pointer h) _NOEXCEPT
    {
        // 우리가 이미 재귀에서 return으로 FixUp을 호출하고 있기 때문에, 재귀적으로 호출할 필요가 없음.

        // Fix right-leaning red nodes.
        if (__isRed(h->right) && !__isRed(h->left))     { h = __rotateLeft(h); }

        // Detect if there is a 4-node that traverses down the left.
        // This is fixed by a right rotation, making both of the red
        // nodes the children of pNode.
        if (__isRed(h->left) && __isRed(h->left->left)) { h = __rotateRight(h); }

        // Split 4-nodes.
        if (__isRed(h->right) && __isRed(h->left))      { __flipColors(h); }

        return h;
    }

private:
    // ------------------------------------------------------------
    //       Utility functions.
    // ------------------------------------------------------------

    // Returns the smallest key in the symbol table.

    // the smallest key in subtree rooted at x; null if no such key
    // * 후속자 노드 찾는데 쓰인다. (node -> right -> left -> left ...)
    // This replacement key is found with __min function
    // by starting at the node's right child, then traversing left until we reach a leaf node.
    // The contents of that leaf node can then be used to replace the value being deleted.
    node_pointer __min(node_pointer x) const _NOEXCEPT
    {
//        assert(x != NULL && "x is null");
        if       (x == NULL)        { return NULL; }
        else if  (x->left == NULL)  { return x; }
        else                        { return __min(x->left); }
    }

    node_pointer __max(node_pointer x) const _NOEXCEPT
    {
//        assert(x != NULL && "x is null");
        if       (x == NULL)        { return NULL; }
        else if  (x->right == NULL) { return x; }
        else                        { return __max(x->right); }
    }








// * 여기서부터는 테스트 후 삭제할 부분 입니다.
// * ---------------------------------------------------------------------------------------------------------------
    // TODO:  Remove this functions later!!
// * ---------------------------------------------------------------------------------------------------------------

public:
    void printTree()
    {
        std::cout << "\n";
        std::cout << "--------------------------------------\n";
        _printTree(m_Root, NULL,  false);
        std::cout << "--------------------------------------\n";
        std::cout << "\n";
    }


private:
    void _printWithColor(const key_type& data, const std::string& color = PRINT_RESET)
    {
        std::cout << " " << color << data.first << PRINT_RESET << std::endl;
    }

    struct Trunk {
        Trunk *prev;
        std::string str;

        Trunk(Trunk *prev, std::string str) {
            this->prev = prev;
            this->str = str;
        }
    };
    // 이진 트리의 분기를 인쇄하는 도우미 함수
    void showTrunks(Trunk *p) {
        if (p == nullptr) {
            return;
        }

        showTrunks(p->prev);
        std::cout << p->str;
    }

    void _printTree(node_pointer root, Trunk *prev, bool isLeft) {
        if (root == nullptr) {
            return;
        }

        std::string prev_str = "    ";
        Trunk *trunk = new Trunk(prev, prev_str);

        _printTree(root->right, trunk, true);

        if (!prev) {
            trunk->str = "———";
        } else if (isLeft) {
            trunk->str = ".———";
            prev_str = "   |";
        } else {
            trunk->str = "`———";
            prev->str = prev_str;
        }

        showTrunks(trunk);

        if (root->color == RED)
            _printWithColor(root->key, PRINT_RED);
        else
            _printWithColor(root->key);

        if (prev) {
            prev->str = prev_str;
        }
        trunk->str = "   |";

        _printTree(root->left, trunk, false);
    }
};



















FT_END_PRIVATE_NAMESPACE
#endif //FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
