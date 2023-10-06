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
    using end_node    = detail::EndNode<key_type>;
    using end_pointer = end_node*;

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

    AVL_Tree(const Compare& comp = Compare())
    : comp_ {comp} {

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

    const_iterator find(const key_type& key) const {
        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                curr_node = curr_node->left_;
            } else if (comp_(curr_node->key_, key)) {
                curr_node = curr_node->right_;
            } else {
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
                    correct_heights(curr_node);
                    break;
                }
                curr_node = curr_node->left_;
            } else if (comp_(curr_node->key_, key)) {
                if (!curr_node->right_) {
                    curr_node->right_ = new node_type{key, curr_node};
                    correct_heights(curr_node);
                    break;
                }
                curr_node = curr_node->right_;
            } else {
                --size_;
                return ;
            }
        }
        balance_tree(curr_node);
        begin_node_ = get_most_left(begin_node_);
    }


    void graph_dump(const std::string& file_name = "graph.png") const {
       graphics::tree_painter<KeyT> graph {end_ptr_};
       graph.graph_dump(file_name);
    }


    size_type size() const noexcept { return size_; };
    
    bool empty() const noexcept { return size_ == 0; };

    const_iterator begin()  const noexcept { return iterator{begin_node_}; }
    const_iterator cbegin() const noexcept { return iterator{begin_node_}; }
    const_iterator end()    const noexcept { return iterator{end_ptr_}; }
    const_iterator cend()   const noexcept { return iterator{end_ptr_}; }
private:
    pointer get_most_left(pointer start_ptr) const {
        while(true) {
            if (start_ptr->left_ == nullptr) {
                return start_ptr;
            }
            start_ptr = start_ptr->left_;
        }
    }

    pointer right_turn(pointer pt) {
        auto parent = pt->parent_;

        if (parent != end_ptr_) {
            (parent->left_ == pt ? parent->left_ : parent->right_) = pt->left_;
            pt->left_->parent_ = parent;
        } else {
            root_node_ = pt->left_;
            root_node_->parent_ = end_ptr_;
            end_ptr_->left_ = root_node_;
        }

        auto tmp = std::exchange(pt->left_, pt->left_->right_);
        if (tmp->right_) {
            tmp->right_->parent_ = pt;
        }
        pt->parent_ = tmp;
        tmp->right_ = pt;

        tmp->right_->height_ = is_child(tmp->right_) ? 0 : determine_height(tmp->right_);
        correct_heights(tmp);

        return tmp;
    }

    pointer left_turn(pointer pt) {
        auto parent = pt->parent_;

        if (parent != end_ptr_) {
            (parent->left_ == pt ? parent->left_ : parent->right_) = pt->right_;
            pt->right_->parent_ = parent;
        } else {
            root_node_ = pt->right_;
            root_node_->parent_ = end_ptr_;
            end_ptr_->left_ = root_node_;
        }

        auto tmp = std::exchange(pt->right_, pt->right_->left_);
        if (tmp->left_) {
            tmp->left_->parent_ = pt;
        }
        pt->parent_ = tmp;
        tmp->left_ = pt;

        tmp->left_->height_ = is_child(tmp->left_) ? 0 : determine_height(tmp->left_);
        correct_heights(tmp);

        return tmp;
    }

    void big_right_turn(pointer pt) {

    }

    void big_left_turn(pointer pt) {

    }

    void correct_heights(pointer pt) {
        while(pt != end_ptr_) {
            pt->height_ = determine_height(pt);
            pt = pt->parent_;
        }
    }
    
    size_type determine_height(pointer pt) const {
        size_type left_h  = height(pt->left_);
        size_type right_h = height(pt->right_);
        return std::max(left_h, right_h) + 1;
    }

    size_type height(pointer pt) const {
        return pt ? pt->height_ : 0;
    }

    void balance_tree(pointer pt) {
        while(pt != end_ptr_) {
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
    }
    
    bool is_child(pointer pt) const {
        return pt->left_ == nullptr && pt->right_ == nullptr;
    }

    difference_type height_difference(const_pointer left, const_pointer right) const noexcept {
        auto res = (left ? (left->height_ + 1) : 0) - (right ? (right->height_ + 1) : 0);
        return res;
    }

    bool is_disbalance(difference_type diff) const noexcept { return diff == DIFF_HEIGHT; }
private:
    pointer root_node_ {nullptr};
    end_node end_node_;
    const end_pointer end_ptr_ {std::addressof(end_node_)};
    pointer begin_node_ {std::addressof(end_node_)};
    Compare comp_;
    size_type size_;
}; // <--- class AVL_Tree

template<typename KeyT, typename Compare>
std::ostream& operator<<(std::ostream& os, const AVL_Tree<KeyT, Compare>& rhs) {
    for (auto val : rhs) {
        os << val << ' ';
    }
    return os << std::endl;
}

} // <--- namespace yLAB

#endif

