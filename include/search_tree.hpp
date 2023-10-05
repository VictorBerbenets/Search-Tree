#ifndef SEARCH_TREE
#define SEARCH_TREE

#include <iostream>
#include <utility>
#include <memory>
#include <initializer_list>
#include <cstddef>

#include "tree_iterator.hpp"
#include "node.hpp"
#include "graph_tree.hpp"

namespace yLAB {

template<typename KeyT = int, typename Compare = std::less<KeyT>>
class AVL_Tree final {
public:
    using key_type        = KeyT;
    using value_type      = KeyT;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare     = Compare;
    using value_compare   = Compare;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using node_type       = detail::Node<key_type>;
    using pointer         = node_type*;
    using const_pointer   = const node_type*;
    using iterator        = detail::TreeIterator<key_type>;
    using const_iterator  = const iterator;
private:
    using end_node        = detail::EndNode<key_type>;
    using end_pointer     = detail::EndNode<key_type>*;

    static constexpr difference_type DIFF_HEIGHT = 2; // difference between two subtree heights
public:
    template <typename Iter>
    AVL_Tree(Iter begin, Iter end, const Compare& comp = Compare())
    : comp_ {comp} {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    AVL_Tree(std::initializer_list<KeyT> ls, const Compare& comp = Compare())
    : AVL_Tree {ls.begin(), ls.end(), comp} {}

    AVL_Tree(const Compare& comp = Compare()) {

    }

    ~AVL_Tree() {
        for (pointer curr_node = root_node_, tmp{0}; curr_node; curr_node = tmp) {
            if (!curr_node->left_) {
                tmp = curr_node->right_;
                delete curr_node;
            } else {
                tmp = curr_node->left_;
                curr_node->left_ = tmp->right_;
                tmp->right_ = curr_node;
            }
        }
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
    const_iterator find(const key_type& key) const {
        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                curr_node = curr_node->left_;
            }
            else if (comp_(curr_node->key_, key)) {
                curr_node = curr_node->right_;
            }
            else {
                return const_iterator{curr_node};
            }
        }
        return cend();
    }

    void insert(const key_type& key) {
        ++size_;
        if (root_node_ == nullptr) {
            root_node_ = new node_type{key};
            end_node_.left_ = root_node_;
            root_node_->parent_ = std::addressof(end_node_);
            begin_node_ = root_node_;
            return;
        }

        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) {
                    curr_node->left_ = new node_type{key, curr_node};
                    correct_height(curr_node);
                    break;
                }
                curr_node = curr_node->left_;
            }
            else if (comp_(curr_node->key_, key)) {
                if (!curr_node->right_) {
                    curr_node->right_ = new node_type{key, curr_node};
                    correct_height(curr_node);
                    break;
                }
                curr_node = curr_node->right_;
            }
            else {
                --size_;
                return ;
            }
        }
        begin_node_ = get_most_left(begin_node_);
        // std::cout << "KEY TO BALANCE = " << curr_node->key_ << std::endl;
#if 0
        if (balance_tree(curr_node) != root_node_) {
            std::cout << "Error" << std::endl;
            std::cout << "ROOT KEY = " << root_node_->key_ << std::endl;
        }
#endif
    }


    void graph_dump(const std::string& file_name = "graph.png") const {
       graphics::tree_painter<KeyT> graph {root_node_};
       graph.graph_dump(file_name);
    }


    size_type size() const noexcept { return size_; };

    const_iterator begin()  const noexcept { return iterator{begin_node_}; }
    const_iterator cbegin() const noexcept { return iterator{begin_node_}; }
    const_iterator end()    const noexcept { return iterator{std::addressof(end_node_)}; }
    const_iterator cend()   const noexcept { return iterator{std::addressof(end_node_)}; }
private:
    //const_pointer root_node() const noexcept { return root_node_; };
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
            size_type left_h  = height(pt->left_);
            size_type right_h = height(pt->right_);
            pt->height_ = std::max(left_h, right_h) + 1;
            pt = pt->parent_;
        }
    }

    pointer balance_tree(pointer pt) {
        while(pt) {
            if ( is_disbalance( height_difference(pt->right_, pt->left_) ) ) {
                if (height_difference(pt->right_->right_, pt->right_->left_) >= 0) {
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
            pt = pt->parent_;
        }
        return pt;
    }

    pointer get_most_left(pointer start_ptr) const {
        while(true) {
            if (start_ptr->left_ == nullptr) {
                return start_ptr;
            }
            start_ptr = start_ptr->left_;
        }
    }

    void right_turn(pointer pt) {

    }

    void big_right_turn(pointer pt) {

    }

    pointer left_turn(pointer pt) {
        auto parent = pt->parent_;
        graph_dump("graph1");
        if (parent) {
            (parent->left_ == pt ? parent->left_ : parent->right_) = pt->right_;
            pt->right_->parent_ = parent;
        } else {
            root_node_ = pt->right_;
            pt->right_->parent_ = nullptr;
        }

        auto tmp     = pt->right_;
        pt->parent_  = tmp;
        pt->right_   = tmp->left_;
        if (pt->right_) {
            if (pt->right_->left_) {
                pt->right_->left_->parent_ = pt;
            }
        }
        tmp->left_   = pt;
        // change heights
        graph_dump("graph2");
        return tmp;
    }

    void big_left_turn(pointer pt) {

    }

    difference_type height_difference(const_pointer left, const_pointer right) const noexcept {
        auto res = (left ? (left->height_ + 1) : 0) - (right ? (right->height_ + 1) : 0);
        return res;
    }

    bool is_disbalance(difference_type diff) const noexcept { return diff == DIFF_HEIGHT; }
private:
    pointer root_node_ {nullptr};
    end_node end_node_;
    pointer begin_node_ {std::addressof(end_node_)};
    Compare comp_;
    size_type size_;
}; // <--- class AVL_Tree

} // <--- namespace yLAB

#endif

