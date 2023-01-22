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
    // _NodeType is <RbNode<T, Compare>.
    typedef typename Allocator::template rebind<_NodeType>::other  node_allocator_type;

    // @ using typename of rebound allocator's typedef
    typedef _NodeType                                           node_type;
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
        return __m_Data_allocator.allocate(sizeof(node_type));
    }

    FT_HIDE_FROM_ABI
    void __deallocate_single_node(pointer _address_of_node)
    {
        if (_address_of_node != NULL) {
            __m_Data_allocator.deallocate(_address_of_node, sizeof(node_type));
        }
    }

    void __construct_node_at(pointer _address_of_node) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_type();
    }

     void __construct_node_by_value_at(pointer _address_of_node, const node_type& _value) {
        // [ placement new ] : construct objects in pre-allocated storage.
        new (_address_of_node) node_type(_value);
    }

    void __destroy_address_of(pointer _address_of_node) {
        _address_of_node->node_type::~node_type(); // directly call destructor.
    }

    Tree_node_alloc_base()
        : __m_Data_allocator(node_allocator_type())
    {}

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
struct RedBlackNode
{

    typedef RedBlackNode<_KeyType, _Compare>     node_type;
    typedef node_type*                           node_pointer;
    typedef _KeyType                             key_type;
    typedef _Compare                             value_compare;

// *-- member data ----------------------------------------------------------------------------
    _KeyType            key;  // key would be std::pair<A, B> if map. if set, key would be single type.
    bool                color;          // color of parent link
    RedBlackNode*       left;
    RedBlackNode*       right;
    RedBlackNode*       parent;         // for iterator system
// *-------------------------------------------------------------------------------------------

    explicit RedBlackNode(_KeyType _key)
            : key(_key), left(NULL), right(NULL), color(RED), /* size(0),*/ parent(NULL)
    {};

    RedBlackNode(_KeyType _key, bool _color)
            : key(_key), color(_color), left(NULL), right(NULL), parent(NULL)
    {}

    RedBlackNode(const node_type& other)
            : key(other.key), color(other.color), left(other.left), right(other.right), parent(other.parent)
    {}

    // if Successor doesn't exist, then return NULL.
    node_pointer getSuccessor()
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
        node_pointer me = this;
        while (above != NULL && me == above->right) // go up if I'm right node of parent.
        {
            me = above;
            above = above->parent;
        }
        return above;
    }

    // if Predecessor doesn't exist, then return NULL.
    node_pointer getPredecessor()
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
        node_pointer me = this;
        while (above != NULL && me == above->left) // go up if I'm left node of parent.
        {
            me = above;
            above = above->parent;
        }
        return above;
    }
};


// TODO:   change this compare class to map_compare.
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

/*
// Forward Declaration for template friend function
template <class _KeyType, class _Compare, class _Allocator>
class LeftLeaningRedBlack;

// Forward Declaration for template friend function
template <typename T>
void printTree(const T& t);
*/


// TODO: use this as RB_base later. // -> add to map / set
// Set and Map must set each template parameter accordingly.

// ? if Map, KeyType will be ft::pair<A, B>.
// ? if Set, KeyType will be A.

// TODO: 아래 기본 값은 나중에 Map Set에서 해주고, 여긴 테스트용으로만 남겨주자.
template <class _KeyType, class _Compare = std::less<_KeyType>, class _Allocator = std::allocator<_KeyType> >
class LeftLeaningRedBlack : protected Tree_node_alloc_base<_KeyType,
                                                           RedBlackNode<_KeyType, _Compare>,
                                                           _Allocator>
{

public: // typedefs
    typedef RedBlackNode<_KeyType, _Compare>                            node_type;
    typedef _KeyType                                                    key_type;
    typedef _Compare                                                    value_compare_type;
    typedef node_type*                                                  node_pointer; // equivalant to node_pointer*
    typedef LeftLeaningRedBlack<key_type, _Compare, _Allocator>         tree_type;

private: // private data member
// *----------------------------------------------
    node_pointer    m_Root;
    _Compare        m_Value_compare; // default compare function class.

    // ! 어떻게 하면 set 과 map에 일관된 compare 함수를 연결할 수 있지?
// *----------------------------------------------

public: // constructor, destructor
    // Initializes an empty symbol table.
    LeftLeaningRedBlack()
        : m_Root(NULL), m_Value_compare(value_compare_type())
        {}

    ~LeftLeaningRedBlack()
    {
        // delete tree every data.
        clear();
        m_Root = NULL;
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

    // ! Node를 destory 하지만, Key는 destory하지 않는다. 이게 중요하다.
    void __destroy_and_deallocate_RbNode(node_pointer p)
    {
        this->__destroy_address_of(p);
        this->__deallocate_single_node(p);
    }

    int __compare_value(const key_type &lhs, const key_type &rhs) const
    {
        if (m_Value_compare(lhs, rhs) == true) { // lhs < rhs
            return 1;
        } else if (m_Value_compare(rhs, lhs) == true) { // lhs > rhs
            return -1;
        }
        return 0; // this == other
    }

    // color is black if node is null.
    bool __isRed(node_pointer x)
    {
        if (x == NULL) return false; // null node is black.
        return (x->color == RED);
    }

    // number of node in subtree rooted at x; 0 if x is null
//    size_t __size(node_pointer x)
//    {
//        if (x == NULL) return 0;
//        return (x->size);
//    }

// ------------------------------------------------------------
//       Standard BST search
// ------------------------------------------------------------
public:
    node_type min()
    {
         if (isEmpty()) throw std::runtime_error("calls min() with empty symbol table");
         return (begin()->key);
    }

    node_pointer begin()
    {
        if (isEmpty()) throw std::runtime_error("calls min() with empty symbol table");
        node_pointer t = __min(m_Root);
        return t;
    }

    node_pointer end() { return NULL; }

    // return true is tree is empty
    bool isEmpty()
    {
        return (m_Root == NULL);
    }

    // find node, which contains target key.
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
            const int cmp = __compare_value(curr->key, target_key);

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

    // insert unique data to red black tree.
    void put(const key_type& key)
    {
        m_Root = __put(m_Root, key);
        m_Root->color = BLACK; // root color must be black.
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
            curr->left->parent = curr; // ! Really needed?
        }
        else if (cmp > 0)   {
            curr->right = __put(curr->right, target_key);
            curr->right->parent = curr; // ! Really needed?
        }
        else                {
            curr->key   = target_key;
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
    //
    // ------------------------------------------------------------
    // delete은 코드가 좀 어렵다.
    // * http://www.teachsolaisgames.com/articles/balanced_left_leaning.html
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
        node_pointer ptr = begin();
        while (ptr != end())
        {
            ptr = ptr->getSuccessor();
            eraseMin();
        }
    }

    // Removes the specified key and its associated value from this symbol table
    // (if the key is in this symbol table).
    // * C++98 versions of Set and Map's erase functions return no value.
    void erase(const key_type& key)
    {
        if (!contains(key)) return;

        m_Root = __erase(m_Root, key);

        // Assuming we have not deleted the last node from the tree, we
        // need to force the root to be a black node to conform with the rules of a red-black tree.
        if (!isEmpty()) { m_Root->color = BLACK; }
    }

private:
    node_pointer __erase(node_pointer curr, const key_type& key)
    {
        if (__compare_value(curr->key, key) < 0)
        // ! if key you want do delete is on the left <--
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
                    // simple swap.
                    // * 후속자 노드를 찾아서 그 값을 복사. (자식이 없을 경우엔 자기 자신 복사)
                    curr->key = (__min(curr->right))->key; // (1) copy successor to current key
                    curr->right = __eraseMin(curr->right); // (2) delete successor node.
                }
                else // ! if key you want do delete is on the right -->
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
    void __flipColors(node_pointer h)
    {
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

    // make a right-leaning link lean to the left
    node_pointer __rotateLeft(node_pointer h)
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
    node_pointer __rotateRight(node_pointer h)
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
    node_pointer __moveRedLeft(node_pointer h)
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
    node_pointer __fixUp(node_pointer h)
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
    node_pointer __min(node_pointer x)
    {
        if (x->left == NULL) { return x; }
        else                 { return __min(x->left); }
    }

private:
    // 아래 friend 함수는 class의 멤버함수가 아니다. 따라서 포함되지 않는다.
    // TODO:  Remove this function later!!
    template<typename U>
    friend void printTree(const LeftLeaningRedBlack<U>& tree);

};

template <typename KeyType>
void _printWithColor(const KeyType& data, const std::string& color = PRINT_RESET)
{
    std::cout << " " << color << data << PRINT_RESET << std::endl;
}





// TODO:  Remove this functions later!!
struct Trunk
{
    Trunk *prev;
    std::string str;

    Trunk(Trunk *prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
    }
};
 // 이진 트리의 분기를 인쇄하는 도우미 함수
void showTrunks(Trunk *p)
{
    if (p == nullptr) {
        return;
    }

    showTrunks(p->prev);
    std::cout << p->str;
}

template <typename KeyType, class Compare>
void _printTree(const RedBlackNode<KeyType, Compare>* root, Trunk *prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }

    std::string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    _printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "———";
    }
    else if (isLeft)
    {
        trunk->str = ".———";
        prev_str =   "   |";
    }
    else {
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

template <typename KeyType>
void printTree(const LeftLeaningRedBlack<KeyType> &tree)
{
    std::cout << "\n";
    std::cout << "--------------------------------------\n";
    _printTree(tree.m_Root, NULL,  false);
    std::cout << "--------------------------------------\n";
    std::cout << "\n";
}

 FT_END_PRIVATE_NAMESPACE
// FT_END_GLOBAL_NAMESPACE

#endif //FT_CONTAINER___LEFT_LEANING_REDBLACK_HPP
