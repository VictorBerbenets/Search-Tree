#ifndef BEST_SEARCH_TREE_FOR_YOUR_MOM_
#define BEST_SEARCH_TREE_FOR_YOUR_MOM_

#include <iostream>
#include <utility>
#include <memory>
#include <stack>
#include <initializer_list>

#include "node.hpp"

namespace yLAB {

template<typename KeyT = int, typename Compare = std::less<KeyT>>
class AVL_Tree final {
    struct Node;

    using size_type       = std::size_t;
    using difference_type = int;
    using pointer         = Node*;
    using const_pointer   = const pointer;
    
    static constexpr difference_type DIFF_HEIGHT = 2; // difference between two subtree heights

    void right_turn(pointer& pt) {

    }

    void big_right_turn(pointer& pt) {

    }

    void left_turn(pointer& pt) {
    
    }

    void big_left_turn(pointer& pt) {

    }
    
    difference_type height_difference(const_pointer& left, const_pointer& right) const noexcept {
        return (left ? left->height_ : 0) - (right ? right->height_ : 0);
    }
    
    bool is_disbalance(difference_type diff) const noexcept { return diff == DIFF_HEIGHT; };
public:
template <typename Iter>
    AVL_Tree(Iter begin, Iter end, const Compare& comp = Compare()): comp_ {comp} {
        for (Iter iter = begin; iter != end; ++iter) {
            insert(*iter);
        }
    }

    AVL_Tree(std::initializer_list<KeyT> ls, const Compare& comp = Compare())
    : AVL_Tree {ls.begin(), ls.end(), comp} {}

    AVL_Tree(const Compare& comp = Compare()) {

    }
    
    ~AVL_Tree() {
        std::stack<pointer> ptrs_stack {};
        pointer curr = root_node_;
        pointer prev {};
        while(curr || !ptrs_stack.empty()) {
            if (curr) {
                ptrs_stack.push(curr);
                curr = curr->left_;
            } else {
                curr = ptrs_stack.top();
                if (!curr->right_ || curr->right_ == prev) {
                    delete curr;
                    ptrs_stack.pop();
                    prev = curr;
                    curr = nullptr;
                } else {
                    curr = curr->right_;
                }
            }
        }
    }
    
    const_pointer ptr() const noexcept { return root_node_; };

    void print(const_pointer pt) {
        if (!pt) { return; }

        if (pt->left_) {
            print(pt->left_);
        }
        std::cout << pt->key_ << std::endl;
        if (pt->right_) {
            print(pt->right_);
        }
    }
    void insert(const KeyT& key) {
        if (root_node_ == nullptr) {
            root_node_ = new Node {key};
            return;
        }
        pointer parent {};
        auto curr_node = root_node_;
        while(curr_node) {
            if (curr_node) { curr_node->height_++; }
            parent = curr_node;
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) { 
                    curr_node->left_ = new Node {key, parent};
                    break; 
                }
                curr_node = curr_node->left_;
            } else if (!comp_(key, curr_node->key_)) {
                if (!curr_node->right_) {
                    curr_node->right_ = new Node {key, parent};
                    break;
                }
                curr_node = curr_node->right_;
            } else { return ; }
        }
    }
    
    void balance_tree(pointer& pt) {
        if ( is_disbalance( height_difference(pt->right, pt->left) ) ) {
            if (height_difference(pt->right->right, pt->right->left) >= 0) {
                left_turn(pt);
            } else {
                big_left_turn(pt);
            }
        } else if ( is_disbalance( height_difference(pt->left, pt->right) ) ) {
            if (height_difference(pt->left->left, pt->left->right) >= 0) {
                right_turn(pt);
            } else {
                big_right_turn(pt);
            } 
        }
    }

    void bypass_tree() const {
        std::stack<Node> st;
    }

private:
    pointer root_node_ {nullptr};
    Compare comp_;

    struct Node {
        Node(KeyT key = 0, pointer parent = nullptr)
        : key_ {key},
          parent_ {parent} {};
    /*--------------------------------------------------------------*/
        KeyT key_;
        pointer parent_ , left_ {nullptr}, right_ {nullptr};
        size_type height_ {0};
    };
};


} // <--- namespace yLAB
#endif

