#ifndef SEARCH_TREE
#define SEARCH_TREE

#include <iostream>
#include <utility>
#include <memory>
#include <initializer_list>
#include <cstddef>
#include <cassert>

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
    using const_iterator  = detail::TreeIterator<key_type>;
private:
    using end_node    = detail::EndNode<key_type>;
    using end_pointer = end_node*;

    enum class childPosition : char {Left = 1, Right = 2};

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
    : comp_ {comp} {}

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
        assert(curr_node);
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

    iterator insert(const key_type& key) {
        if (root_node_ == nullptr) { return create_root_node(key); }

        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) {
                    create_node(curr_node, key, childPosition::Left);
                    break;
                }
                curr_node = curr_node->left_;
            } else if (comp_(curr_node->key_, key)) {
                if (!curr_node->right_) {
                    create_node(curr_node, key, childPosition::Right);
                    break;
                }
                curr_node = curr_node->right_;
            } else {
                return iterator{curr_node};
            }
        }

        balance_tree(curr_node);
        begin_node_ = get_most_left(begin_node_);

        return iterator{curr_node};
    }

    size_type erase(const key_type& key) {
        size_type save_size = size();
        node_type tmp_node{key};
        erase(const_iterator{&tmp_node});
        return size() != save_size;
    }

    iterator erase(const_iterator pos) {
        auto erase_it = find(*pos);
        if (erase_it == end()) { return end(); }

        auto erase_ptr     = const_cast<pointer>(erase_it.get_pointer());
        auto replace_ptr   = erase_ptr->left_ ? erase_ptr->get_most_right() : nullptr;
        iterator next_iter = ++iterator{erase_ptr};
        if (replace_ptr) {
            erase_ptr->key_ = replace_ptr->key_;
            erase_node(replace_ptr, replace_ptr->left_);
        } else {
            erase_node(erase_ptr, erase_ptr->right_);
        }
        begin_node_ = get_most_left(end_ptr_);
      //  std::cout << *this << std::endl;
        --size_;
        return next_iter;
    }

    iterator erase(const_iterator begin, const_iterator end) {
        iterator tmp{nullptr};
        for (; begin != end; ++begin) {
            tmp = erase(begin);
        }
        return tmp;
    }

    void graph_dump(const std::string& file_name = "graph.png") const {
       graphics::tree_painter<KeyT> graph {end_ptr_};
       graph.graph_dump(file_name);
    }

    size_type size() const noexcept { return size_; };
    bool empty() const noexcept     { return size_ == 0; };

    const_iterator begin()  const noexcept { return iterator{begin_node_}; }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator end()    const noexcept { return iterator{end_ptr_}; }
    const_iterator cend()   const noexcept { return end(); }
private:
    pointer right_turn(pointer pt) {
        set_child_parent_connection(pt, pt->left_);

        auto local_child = std::exchange(pt->left_, pt->left_->right_);

        if (local_child->right_) {
            local_child->right_->parent_ = pt;
        }
        pt->parent_ = local_child;
        local_child->right_ = pt;

        correct_heights(local_child->right_);

        return local_child;
    }

    pointer left_turn(pointer pt) {
        set_child_parent_connection(pt, pt->right_);

        auto local_child = std::exchange(pt->right_, pt->right_->left_);

        if (local_child->left_) {
            local_child->left_->parent_ = pt;
        }
        pt->parent_ = local_child;
        local_child->left_ = pt;

        correct_heights(local_child->left_);

        return local_child;
    }

    pointer big_right_turn(pointer pt) {
        set_child_parent_connection(pt, pt->left_->right_);

        auto local_root = pt->left_;
        auto local_root_child = local_root->right_;

        pt->left_ = local_root_child->right_;
        if (local_root_child->right_) {
            local_root_child->right_->parent_ = pt;
        }
        local_root->right_ = local_root_child->left_;
        if (local_root_child->left_) {
            local_root_child->left_->parent_ = local_root;
        }

        local_root_child->right_ = pt;
        local_root_child->left_  = local_root;

        pt->parent_ = local_root->parent_ = local_root_child;

        set_child_height(local_root_child->right_);
        correct_heights(local_root_child->left_);

        return local_root_child;
    }

    pointer big_left_turn(pointer pt) {
        set_child_parent_connection(pt, pt->right_->left_);

        auto local_root = pt->right_;
        auto local_root_child = local_root->left_;

        pt->right_ = local_root_child->left_;
        if (local_root_child->left_) {
            local_root_child->left_->parent_ = pt;
        }
        local_root->left_ = local_root_child->right_;
        if (local_root_child->right_) {
            local_root_child->right_->parent_ = local_root;
        }

        local_root_child->left_  = pt;
        local_root_child->right_ = local_root;

        pt->parent_ = local_root->parent_ = local_root_child;

        set_child_height(local_root_child->right_);
        correct_heights(local_root_child->left_);

        return local_root_child;
    }

    void set_child_height(pointer child) {
        child->height_ = is_child(child) ? 0 : determine_height(child);
    }

    void set_child_parent_connection(pointer pt, pointer child) {
        auto parent = pt->parent_;

        if (parent != end_ptr_) {
            (parent->left_ == pt ? parent->left_ : parent->right_) = child;
            child->parent_ = parent;
        } else {
            root_node_ = child;
            root_node_->parent_ = end_ptr_;
            end_ptr_->left_ = root_node_;
        }
    }

    void correct_heights(pointer pt) {
        while(pt != end_ptr_) {
            pt->height_ = determine_height(pt);
            pt = pt->parent_;
        }
    }

    size_type determine_height(pointer pt) const {
        if (!pt->left_ && !pt->right_) {
            return 0;
        }
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
                    pt = big_left_turn(pt);
                }
            } else if ( is_disbalance( height_difference(pt->left_, pt->right_) ) ) {
                if (height_difference(pt->left_->left_, pt->left_->right_) >= 0) {
                    pt = right_turn(pt);
                } else {
                    pt = big_right_turn(pt);
                }
            }
            pt = pt->parent_;
        }
    }

    difference_type height_difference(const_pointer left, const_pointer right) const noexcept {
        return (left ? (left->height_ + 1) : 0) - (right ? (right->height_ + 1) : 0);
    }

    bool is_disbalance(difference_type diff) const noexcept { return diff == DIFF_HEIGHT; }

    void erase_node(pointer& erase_ptr, pointer child) { // erase_ptr doesn't have at least one child
        auto erase_parent = erase_ptr->parent_;
        if (child) {
            (erase_parent->left_ == erase_ptr ? erase_parent->left_ : erase_parent->right_) = child;
            child->parent_ = erase_parent;
        } else {
            (erase_parent->left_ == erase_ptr ? erase_parent->left_ : erase_parent->right_) = nullptr;
        }
        if (erase_ptr == root_node_) {
            root_node_ = child;
        }
        delete erase_ptr;
        correct_heights(erase_parent);
        balance_tree(erase_parent);
    }

    iterator create_root_node(const key_type& key) {
        root_node_ = new node_type{key};
        end_node_.left_ = root_node_;
        root_node_->parent_ = end_ptr_;
        begin_node_ = root_node_;

        return iterator{root_node_};
    }

    void create_node(pointer curr_node, const key_type& key, childPosition pos) {
        if (pos == childPosition::Left) {
            curr_node->left_ = new node_type{key, curr_node};
        } else {
            curr_node->right_ = new node_type{key, curr_node};
        }
        correct_heights(curr_node);
        ++size_;
    }

    static bool is_child(pointer pt) {
        assert(pt);
        return pt->left_ == nullptr && pt->right_ == nullptr;
    }

    static pointer get_most_left(pointer start_ptr) {
        assert(start_ptr);
        while(true) {
            if (start_ptr->left_ == nullptr) {
                return start_ptr;
            }
            start_ptr = start_ptr->left_;
        }
    }
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

