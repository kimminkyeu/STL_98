//
// Created by 김민규 on 2023/01/20.
//

#ifndef FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
#define FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP

#include <map>
#include <set>
#include "__config.hpp"

FT_BEGIN_PRIVATE_NAMESPACE


// ---------------------------------------------------------------
// |                                                             |
// |             Tree_node_alloc_base implementation             |
// |                                                             |
// ---------------------------------------------------------------
// wrapper class to handle all the node allocating, rebind, etc...
// 주어진 T를 각 tree에 맞는 node로 묶어서 하나의 단위로 allocate.
// ! -> RB-tree에선 RB-Node를 연결해주면 된다.

/*  allocator에 있는 rebind 구조체.
template<class Other>
struct rebind {
    typedef allocator<Other> other;
};
*/


// if it is map, T would be std::pair<Key, Value>
template <class T, class _NodeType_, class Allocator>
class Tree_node_alloc_base
{
protected: // typedef and namespace scope

    // @ rebinding allocator<T> to allocator<node<T>>
    typedef typename Allocator::template rebind<_NodeType_>::other  node_allocator_type;

    // @ using typename of rebound allocator's typedef
    typedef typename node_allocator_type::size_type		        size_type;
    typedef typename node_allocator_type::pointer		        pointer;
    typedef typename node_allocator_type::const_pointer		    const_pointer;
    typedef typename node_allocator_type::reference		        reference;
    typedef typename node_allocator_type::const_reference		const_reference;
    typedef typename node_allocator_type::difference_type		difference_type;


protected:
    // ------------------------------
    node_allocator_type m_Data_allocator;
    // ------------------------------

    FT_HIDE_FROM_ABI // allocate single node. same as ::new node();
    pointer _allocate_single_node()
    {
        return m_Data_allocator.allocate(sizeof(_NodeType_));
    }

    FT_HIDE_FROM_ABI
    void _deallocate_single_node(pointer _address_of_node)
    {
        if (_address_of_node != NULL)
        {
            m_Data_allocator.deallocate(_address_of_node, sizeof(_NodeType_));
        }
    }

    FT_HIDE_FROM_ABI
    explicit Tree_node_alloc_base(const node_allocator_type& _a)
            : m_Data_allocator(_a)
    {}

    // * *************************************************************************
    // TODO: alloc_base의 소멸자에서 Tree를 후위순회하면서 노드들을 전부 제거해주는게 맞지 않을까?
    // * *************************************************************************

};






// ---------------------------------------------------------------
// |                                                             |
// |            Red-Black Tree Node implementation               |
// |                                                             |
// ---------------------------------------------------------------
// Ref: https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/RedBlackBST.html

const bool RED = false;
const bool BLACK = true;

// Node for Red Black tree
template <typename KeyType>
struct _RedBlackNode {

// *-------------------------------------------------------------------------------------------
    KeyType     key;  // key would be std::pair<A, B> if map. if set, key would be single type.
// *-------------------------------------------------------------------------------------------

    bool            color;          // color of parent link
    size_t          size;           // subtree count
    _RedBlackNode*   left, right;    // link to left for right subtree

    explicit _RedBlackNode(KeyType _key)
            : key(_key), left(NULL), right(NULL), color(RED), size(0)
    {};

    _RedBlackNode(KeyType _key, bool _color, size_t _size)
            : key(_key), left(NULL), right(NULL), color(_color), size(_size)
    {}
};



// ---------------------------------------------------------------
// |                                                             |
// |        Left Leaning Red-Black Tree implementation           |
// |                                                             |
// ---------------------------------------------------------------
// TODO: use this as RB_base later. // -> add to map / set
// Set and Map must set each template parameter accordingly.

// ! if Map, T will be ft::pair<A, B>.
// ! if Set, T will be ft::pair<A, A>.
template <class _PairType, class Compare, class Allocator> //-> map 구현시 std::allocator default로 설정.
class LeftLeaningRedBlack : protected Tree_node_alloc_base<_PairType, _RedBlackNode<_PairType>, Allocator>
{
public: // typedefs
    typedef _RedBlackNode<_PairType>          node_type;
    typedef _PairType                         pair_type;


private: // private data member
// *----------------------------------------------
    node_type* _root;
// *----------------------------------------------



public: // constructor, destructor
    // Initializes an empty symbol table.
    LeftLeaningRedBlack()
        : _root(NULL)
    {}


// ------------------------------------------------------------
//       internal helper functions.
// ------------------------------------------------------------
private:
    // color is black if node is null.
    bool isRed(node_type* x)
    {
        if (x == NULL) return false; // null node is black.
        return (x->color == RED);
    }

    // number of node in subtree rooted at x; 0 if x is null
    size_t size(node_type* x)
    {
        if (x == NULL) return 0;
        return (x->size);
    }

    size_t size()
    {
        return size(_root);
    }

public:
    bool isEmpty()
    {
        return (_root == NULL);
    }

// ------------------------------------------------------------
//       Standard BST search
// ------------------------------------------------------------
public:
/**
     * Returns the value associated with the given key.
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     *     and {@code null} if the key is not in the symbol table
     * @throws IllegalArgumentException if {@code key} is {@code null}
     */


    node_type*  flipColors(node_type* h)
    {
        // h must have opposite color of it's two children.
        // assert (h != null) && (h.left != null) && (h.right != null));
        // assert (!isRed(h) &&  isRed(h.left) &&  isRed(h.right))
        //    || (isRed(h)  && !isRed(h.left) && !isRed(h.right));
        h->color = !(h->color);
    }







};

















FT_END_PRIVATE_NAMESPACE

#endif //FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
