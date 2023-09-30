#ifndef BEST_SEARCH_TREE_FOR_YOUR_MOM_
#define BEST_SEARCH_TREE_FOR_YOUR_MOM_

#include <iostream>
#include <utility>
#include <memory>
#include <stack>
#include <initializer_list>

#include "node.hpp"
#include "graph_tree.hpp"

namespace yLAB {

template<typename KeyT = int, typename Compare = std::less<KeyT>>
class AVL_Tree final {

    using size_type       = std::size_t;
    using difference_type = int;
    using pointer         = Node<KeyT>*;
    using const_pointer   = const Node<KeyT>*;

    static constexpr difference_type DIFF_HEIGHT = 2; // difference between two subtree heights

    void right_turn(pointer pt) {

    }

    void big_right_turn(pointer pt) {

    }

    pointer left_turn(pointer pt) {
        auto& parent = pt->parent_;
        std::cout << "KEY = " << pt->key_ << std::endl;
            std::cout << "---------------------TREE BEFORE----------------------\n";
            print(ptr());
            std::cout << "------------------------------------------------------\n";
        graph_dump("graph1");
        if (parent) {
            parent->left_ == pt ? parent->left_ : parent->right_ = pt->right_;
            pt->right_->parent_ = parent;
        } else {
            root_node_ = pt->right_;
            pt->right_->parent_ = nullptr;
        }

        auto tmp     = pt->right_;
        pt->parent_  = pt->right_;
        pt->right_   = pt->right_->left_;
        if (pt->right_) {
            if (pt->right_->left_) {
                pt->right_->left_->parent_ = pt;
            }
        }
        tmp->left_   = pt;
        /*if (parent) {
            tmp->parent_ = parent;
        } else {
            tmp->parent_ = nullptr;
        }*/
        graph_dump("graph2");
       // correct_height(tmp);
        //correct_height(pt);
            std::cout << "---------------------TREE AFTER----------------------\n";
            print(ptr());
            std::cout << "------------------------------------------------------\n";
        return tmp;
    }

    void big_left_turn(pointer pt) {

    }
    
    difference_type height_difference(const_pointer left, const_pointer right) const noexcept {
        auto res = (left ? (left->height_ + 1) : 0) - (right ? (right->height_ + 1) : 0);
        std::cout << "LEFT HEIGHT  = " << (left ? (left->height_ + 1) : 0) << std::endl;
        std::cout << "RIGHT HEIGHT = " << (right ? (right->height_ + 1): 0) << std::endl;
#if 0
        std::cout << "IN HEIGHT DIFFERENCE:" << std::endl;
        std::cout << "DIFF = " << res << std::endl;
#endif
        return res;
        //return (left ? left->height_ : 0) - (right ? right->height_ : 0);
    }

    bool is_disbalance(difference_type diff) const noexcept { /*std::cout << "DIFF = " << diff << std::endl;*/ return diff == DIFF_HEIGHT; };
public:
template <typename Iter>
    AVL_Tree(Iter begin, Iter end, const Compare& comp = Compare()): comp_ {comp} {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    AVL_Tree(std::initializer_list<KeyT> ls, const Compare& comp = Compare()): AVL_Tree {ls.begin(), ls.end(), comp} {}

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
    
    void Graph_dump() {
        
    }

    void print(const_pointer pt) {
        if (!pt) { return; }
        
        std::cout << "key = " << pt->key_ << std::endl;
        std::cout << "height = " << pt->height_ << std::endl;

        if (pt->left_) {
            print(pt->left_);
        }
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
    size_type height(const_pointer pt) const {
        return pt ? pt->height_ : 0;
    }

    void correct_height(pointer pt) {
        while(pt) {
#if 0
            std::cout << "pt     = " << pt << std::endl;
            std::cout << "pt par = " << pt->parent_ << std::endl;
            std::cout << "CORRECTING\n";
#endif
            size_type left_h  = height(pt->left_);
            size_type right_h = height(pt->right_);
            pt->height_ = (left_h > right_h ? left_h: right_h) + 1;
            pt = pt->parent_;
        }
    }

    void insert(const KeyT& key) {
      //  std::cout << "KEY TO INSERT = " << key << std::endl;
        if (root_node_ == nullptr) {
            root_node_ = new Node {key};
            return;
        }
        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) { 
                    curr_node->left_ = new Node {key, curr_node};
                    correct_height(curr_node);
                    break; 
                }
                curr_node = curr_node->left_;
            } 
            else if (comp_(curr_node->key_, key)) {
                if (!curr_node->right_) {
                    curr_node->right_ = new Node {key, curr_node};
                    correct_height(curr_node);
                    break;
                }
                curr_node = curr_node->right_;
            }
            else { return ; }
        }
        // std::cout << "KEY TO BALANCE = " << curr_node->key_ << std::endl;
        if (balance_tree(curr_node) != root_node_) {
            std::cout << "Error" << std::endl;
        }
    }
    
    pointer balance_tree(pointer pt) {
        while(pt) {
            std::cout << "---CICLE KEY           = " << pt->key_ << std::endl;
            std::cout << "---CHECKING KEY HEIGHT = " << pt->height_ << std::endl;
           // std::cout << "---PT BEFORE IF = " << pt << std::endl;
            if ( is_disbalance( height_difference(pt->right_, pt->left_) ) ) {
                std::cout << "IS DISBALANCE ON KEY = " << pt->key_ << std::endl;
                if (height_difference(pt->right_->right_, pt->right_->left_) >= 0) {
                    std::cout << "LEFT TURN\n";
//            std::cout << "key = " << pt->key_ << std::endl;
                    pt = left_turn(pt);
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
           // std::cout << "---PT AFTER IF = " << pt << std::endl;
            pt = pt->parent_;
        }
        return pt;
    }

    void bypass_tree() const {
       // std::stack<> st;
    }

    void graph_dump(const std::string& file_name = "graph.png") const {
       graphics::tree_painter<KeyT> graph {root_node_};
       graph.graph_dump(file_name);
    }

private:
    pointer root_node_ {nullptr};
    Compare comp_;
};


} // <--- namespace yLAB
#endif

