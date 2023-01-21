//
// Created by 김민규 on 2023/01/20.
//

#ifndef FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
#define FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP

// #include <functional>
// #include <map>
// #include <set>
#include "__config.hpp"
#include <exception>
#include <stdexcept>

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
template <class T, class _NodeType, class Allocator>
class Tree_node_alloc_base
{
protected: // typedef and namespace scope

    // @ rebinding allocator<T> to allocator<node<T>>
    typedef typename Allocator::template rebind<_NodeType>::other  node_allocator_type;

    // @ using typename of rebound allocator's typedef
    typedef _NodeType                                           node_pointer;
    typedef typename node_allocator_type::size_type		        size_type;
    typedef typename node_allocator_type::pointer		        pointer;
    typedef typename node_allocator_type::const_pointer		    const_pointer;
    typedef typename node_allocator_type::reference		        reference;
    typedef typename node_allocator_type::const_reference		const_reference;
    typedef typename node_allocator_type::difference_type		difference_type;


protected:
    // ------------------------------
    node_allocator_type __m_Data_allocator;
    // ------------------------------

    FT_HIDE_FROM_ABI // allocate single node. same as ::new node();
    pointer __allocate_single_node()
    {
        return __m_Data_allocator.allocate(sizeof(node_pointer));
    }

    FT_HIDE_FROM_ABI
    void __deallocate_single_node(pointer _address_of_node)
    {
        if (_address_of_node != NULL) {
            __m_Data_allocator.deallocate(_address_of_node, sizeof(node_pointer));
        }
    }

    void __construct_node_at(pointer _address_of_node) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_pointer();
    }

     void __construct_node_by_value_at(pointer _address_of_node, const node_pointer& _value) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_pointer(_value);
    }

    void __destroy_address_of(pointer _address_of_node) {
        _address_of_node->node_pointer::~node_pointer(); // directly call destructor.
    }


    FT_HIDE_FROM_ABI
    explicit Tree_node_alloc_base(const node_allocator_type& _a)
            : __m_Data_allocator(_a)
    {}

    // * *************************************************************************
    // TODO: alloc_base의 소멸자에서 Tree를 후위순회하면서 노드들을 전부 제거해주는게 맞지 않을까?
    // * *************************************************************************

};


// --------------------------------------------------------------------------
// |                                                                        |
// |          Left Leaning Red Black tree's Node implementation             |
// |                                                                        |
// --------------------------------------------------------------------------

const bool RED = false;
const bool BLACK = true;

// Node for Red Black tree
// passed template parameter _Compare for __key_compare member function.
template <typename _KeyType, class _Compare>
struct _RedBlackNode
{

    typedef _RedBlackNode<_KeyType, _Compare>    node_pointer;
    typedef _KeyType                             key_type;
    typedef _Compare                             value_compare;

// *-- member data ----------------------------------------------------------------------------
    _KeyType            key;  // key would be std::pair<A, B> if map. if set, key would be single type.
    bool                color;          // color of parent link
    size_t              size;           // subtree count
    _RedBlackNode*      left, right;    // link to left for right subtree
// *-------------------------------------------------------------------------------------------

    explicit _RedBlackNode(_KeyType _key)
            : key(_key), left(NULL), right(NULL), color(RED), size(0)
    {};

    _RedBlackNode(_KeyType _key, bool _color, size_t _size)
            : key(_key), left(NULL), right(NULL), color(_color), size(_size)
    {}

    _RedBlackNode(const node_pointer& other)
            : key(other._key), left(other.left), right(other.right), color(other.color), size(other.size)
    {}

    // Wrapper private function to handle equality(==) check.
    // std::less only provides (<) operator.
    int compare_node_to(const node_pointer& other_node) const
    {
        return compare_key_to(other_node.key);
    }

    // usage:: node.__compare_key_to(x.key)
    int compare_key_to(const key_type& other_key) const
    {
        if (value_compare(this->key, other_key) == true) { // this < other
            return 1;
        } else if (value_compare(other_key, this->key) == true) { // this > other
            return -1;
        }
        return 0; // this == other
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




// TODO: use this as RB_base later. // -> add to map / set
// Set and Map must set each template parameter accordingly.

// ? if Map, KeyType will be ft::pair<A, B>.
// ? if Set, KeyType will be A.

template <class _KeyType, class _Compare, class _Allocator> //-> map 구현시 std::allocator default로 설정.
class LeftLeaningRedBlack : protected Tree_node_alloc_base<_KeyType,
                                                           _RedBlackNode<_KeyType, _Compare>,
                                                           _Allocator>
{
public: // typedefs
    typedef _RedBlackNode<_KeyType, _Compare>                       node_type;
    typedef _KeyType                                                key_type;
    typedef _Compare                                                value_compare;

    typedef Tree_node_alloc_base<node_type, key_type, _Allocator>   _node_Alloc_base;
    typedef typename _node_Alloc_base::node_allocator_type          node_allocator_type;

    typedef node_type*                   node_pointer; // equivalant to node_pointer*
    // ! 원래 아래껄 썼으나, 포인터 사용이 힘들어서 위처럼 바꿈.
    // typedef typename node_allocator_type::pointer                   node_pointer; // equivalant to node_pointer*
    typedef typename node_allocator_type::size_type                 size_type;

private: // private data member
// *----------------------------------------------
    node_pointer m_Root;
// *----------------------------------------------

public: // constructor, destructor
    // Initializes an empty symbol table.
    LeftLeaningRedBlack()
        : m_Root(NULL) {}

// ------------------------------------------------------------
//       RB Node helper method.
// ------------------------------------------------------------

private:
    node_pointer __allocate_and_construct_RbNode(const key_type& _key, const bool& _color, const size_t& _size)
    {
        node_pointer* address_of_new_node = this->__allocate_single_node();
        this->__construct_node_by_value_at(address_of_new_node, node_pointer(_key, _color, _size));
        return address_of_new_node;
    }

    // color is black if node is null.
    bool __isRed(node_pointer* x)
    {
        if (x == NULL) return false; // null node is black.
        return (x->color == RED);
    }

    // number of node in subtree rooted at x; 0 if x is null
    size_type __size(node_pointer* x)
    {
        if (x == NULL) return 0;
        return (x->size);
    }

    size_type __size()
    {
        return __size(m_Root);
    }

// ------------------------------------------------------------
//       Standard BST search
// ------------------------------------------------------------
public:
    // return true is tree is empty
    bool isEmpty()
    {
        return (m_Root == NULL);
    }

public:
    // throw exception is key_type is null
    node_pointer getNode(const key_type& target_key)
    {
        return __getNode(m_Root, target_key);
    }

private:
    // because std::less only returns true or false, we need to handle equal value.
    node_pointer __getNode(node_pointer curr, const key_type& target_key)
    {
        while (curr != NULL)
        {
            const int cmp = curr->__compare_key_to(target_key);

            if      (cmp < 0)   curr = curr->left;
            else if (cmp > 0)   curr = curr->right;
            else                return curr;
        }
        return NULL; // key not found
    }

public:
    // Does this symbol table contain the given key?
    bool contains(const key_type& key)
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

private:
    node_pointer __put(node_pointer curr, const key_type& target_key)
    {
        // if empty tree
        if (curr == NULL) {
             return (__allocate_and_construct_RbNode(target_key, RED, 1));
        }
        // if not empty tree, then recursively insert data.
        const int cmp = curr->compare_key_to(target_key);

        if      (cmp < 0)   curr->left  = put(curr->left, target_key);
        else if (cmp > 0)   curr->right = put(curr->right, target_key);
        else                curr->key   = target_key;

        // ----------------------------------------
        // |    Fix-up any right-leaning links    |
        // ----------------------------------------

        // * Case 1. right leaning red
        /*
                   0
                 / |
                /  |
               0   O -> Red                                 */
        if (__isRed(curr->right) && !__isRed(curr->left)) {
             curr = __rotateLeft(curr);
        }

        // * Case 2. double left red
        /*
                   0
                  /    -->   if left and left's left is red
            Red O
               /
          Red O                                              */
        if (__isRed(curr->left) && __isRed(curr->left->left)) {
             curr = __rotateRight(curr);
        }

        // * Case 3. two red child
        /*
                        0
                      / |
                     /  |
             Red <- O   O -> Red                             */
        if (__isRed(curr->right) && __isRed(curr->left)) {
             curr = __flipColors(curr);
        }

        curr->size = __size(curr->left) + __size(curr->right) + 1;

        return curr;
    }

    // ------------------------------------------------------------
    //       Red Black tree deletion
    // ------------------------------------------------------------

private:
    // Removes the smallest key and associated value from the symbol table.
    void __eraseMin()
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
        // ! 어디에서 삭제 로직이 없는데 어찌 된겨?
    }

private:
    node_pointer __eraseMin(node_pointer curr)
    {
        if (curr->left == NULL) {
            return NULL;
        }

        if (!__isRed(curr->left) && !__isRed(curr->right)) {
            curr = __moveRedLeft(curr);
        }

        curr->left = __eraseMin(curr->left);

        return __balance(curr);
    }

private:
    // Removes the largest key and associated value from the symbol table
    void __eraseMax()
    {
        if (isEmpty()) throw std::runtime_error("BST underflow");

        // if both children of root are black, set root to red.
        if (!__isRed(m_Root->left) && !__isRed(m_Root->right)) {
            m_Root->color = RED;
        }

        m_Root = __eraseMax(m_Root);
        if (!isEmpty()) {
            m_Root->color = BLACK;
        }
    }

private:
    // delete the key-value pair with the maximum key rooted at h
    node_pointer __eraseMax(node_pointer curr)
    {
        if (__isRed(curr->left)) {
            curr = __rotateRight(curr);
        }


        if (curr->right == NULL) {
            return NULL;
        }

        if (!__isRed(curr->left) && !__isRed(curr->right)) {
            curr = __moveRedRight(curr);
        }

        curr->right = __eraseMax(curr->left);

        return __balance(curr);
    }

public:
    // Removes the specified key and its associated value from this symbol table
    // (if the key is in this symbol table).
    void erase(const key_type& key)
    {
        if (!contains(key)) return;

        // if both children of root are black, set root to red.
        if (!__isRed(m_Root->left) && !__isRed(m_Root->right)) {
            m_Root->color = RED;
        }

        m_Root = erase(m_Root, key);
        if (!isEmpty()) {
            m_Root->color = BLACK;
        }
    }

private:
    node_pointer __erase(node_pointer curr, const key_type& key)
    {
        if (curr->compare_key_to(key) < 0)
        {
            if (!__isRed(curr->left) && !__isRed(curr->left->left)) {
                curr = __moveRedLeft(curr);
            }
            curr->left = __erase(curr->left, key);
        }
        else
        {
            if (__isRed(curr->left)) {
                curr = __rotateRight(curr);
            }
            if (curr->compare_key_to(key) == 0 && (curr->right == NULL)) {
                return NULL;
            }
            if (!__isRed(curr->right) && !__isRed(curr->right->left)) {
                curr = __moveRedRight(curr);
            }
            if (curr->compare_key_to(key) == 0) {
                // simple swap.
                curr->key = (__min(curr->right))->key;
                curr->right = __eraseMin(curr->right);
            }
            else {
                curr->right = __erase(curr->right, key);
            }
        }
        return __balance(curr);
    }







private:
    // ------------------------------------------------------------
    //       Red Black tree helper functions.
    // ------------------------------------------------------------

    // flip the colors of a node and its two children
    node_pointer __flipColors(node_pointer h) {
    }

    // make a left-leaning link lean to the right
    node_pointer __rotateRight(node_pointer h) {}

    // make a right-leaning link lean to the left
    node_pointer __rotateLeft(node_pointer h) {}

    // Assuming that h is red and both h.left and h.left.left
    // are black, make h.left or one of its children red.
    node_pointer __moveRedLeft(node_pointer h) {}

    // Assuming that h is red and both h.right and h.right.left
    // are black, make h.right or one of its children red.
    node_pointer __moveRedRight(node_pointer h) {}

    // restore red-black tree invariant
    node_pointer __balance(node_pointer h) {}

private:
    // ------------------------------------------------------------
    //       Utility functions.
    // ------------------------------------------------------------

    // Returns the smallest key in the symbol table.
    key_type __min() {}

    // the smallest key in subtree rooted at x; null if no such key
    node_pointer __min(node_pointer x) {}

    // Returns the largest key in the symbol table.
    key_type __max() {}

    // the largest key in the subtree rooted at x; null if no such key
    node_pointer __max(node_pointer x) {}

    // Returns the largest key in the symbol table less than or equal to {@code key}
    key_type __floor(const key_type& key) {}

    // the largest key in the subtree rooted at x less than or equal to the given key
    node_pointer __floor(node_pointer x, const key_type& key) {}

    // Returns the smallest key in the symbol table greater than or equal to {@code key}.
    key_type __ceiling(const key_type& key) {}

    // the smallest key in the subtree rooted at x greater than or equal to the given key
    node_pointer __ceiling(node_pointer x, const key_type& key) {}

    //  Return the key in the symbol table of a given {@code rank}.
    //      This key has the property that there are {@code rank} keys in
    //      the symbol table that are smaller. In other words, this key is the
    //      ({@code rank}+1)st smallest key in the symbol table.
    key_type __select(const int& rank) {}

    // Return key in BST rooted at x of given rank.
    // Precondition: rank is in legal range.
    key_type __select(node_pointer x, const int& rank) {}

    //  Return the number of keys in the symbol table strictly less than {@code key}.
    int __rank(const key_type& key) {}

    // number of keys less than key in the subtree rooted at x
    int __rank(const key_type& key, node_pointer x) {}


    // ------------------------------------------------------------
    //       Range Count and Rande Search.
    // ------------------------------------------------------------
    public:
















};

















FT_END_PRIVATE_NAMESPACE

#endif //FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
