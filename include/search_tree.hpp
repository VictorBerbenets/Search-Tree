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

    void right_turn(pointer pt) {

    }

    void big_right_turn(pointer pt) {

    }

    void left_turn(pointer pt) {
        auto& parent = pt->parent_;
        if (parent) {
            parent->left_ == pt ? parent->left_ : parent->right_ = pt->right_;
        }

        auto tmp     = pt->right_;
        pt->parent_  = pt->right_;
        pt->right_   = pt->right_->left_;
        tmp->left_   = pt;
        tmp->parent_ = parent;
        pt = tmp;
#if 0
        auto right_left_subtree = pt->right_->left_;
        std::swap(right
        pt = tmp->right;
        std::swap()
#endif
    }

    void big_left_turn(pointer pt) {

    }
    
    difference_type height_difference(const_pointer left, const_pointer right) const noexcept {
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
        std::cout << "key = " << pt->key_ << std::endl;
        std::cout << "height = " << pt->height_ << std::endl;
        if (pt->right_) {
            print(pt->right_);
        }
    }

    void increase_heights(pointer pt) {
        while(pt) {
            pt->height_++;
            pt = pt->parent_;
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
//            if (curr_node) { curr_node->height_++; }
            parent = curr_node;
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) { 
                    curr_node->left_ = new Node {key, parent};
                    increase_heights(curr_node);
                    break; 
                }
                curr_node = curr_node->left_;
            } else if (key == curr_node->key_) {
                return ;
            } else {
                if (!curr_node->right_) {
                    curr_node->right_ = new Node {key, parent};
                    increase_heights(curr_node);
                    break;
                }
                curr_node = curr_node->right_;
            }
        }
        balance_tree(curr_node);
    }
    
    void balance_tree(pointer& pt) {
        while(pt) {
            std::cout << "TREE\n";
            print(ptr());
            std::cout << "pt = " << pt << std::endl;
            std::cout << "key = " << pt->key_ << std::endl;
            if ( is_disbalance( height_difference(pt->right_, pt->left_) ) ) {
                std::cout << "IS DISBALANCE\n";
                if (height_difference(pt->right_->right_, pt->right_->left_) >= 0) {
                    std::cout << "LEFT TURN\n";
            std::cout << "key = " << pt->key_ << std::endl;
                    left_turn(pt);
                } else {
                    big_left_turn(pt);
                }
            } else if ( is_disbalance( height_difference(pt->left_, pt->right_) ) ) {
                if (height_difference(pt->left_->left_, pt->left_->right_) >= 0) {
                    right_turn(pt);
                } else {
                    big_right_turn(pt);
                } 
            }
            pt = pt->parent_;
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

        ~Node() = default;
    /*--------------------------------------------------------------*/
        KeyT key_;
        pointer parent_ , left_ {nullptr}, right_ {nullptr};
        size_type height_ {0};
    };
};


} // <--- namespace yLAB
#endif

