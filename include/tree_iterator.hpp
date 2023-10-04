#ifndef TREE_ITERATOR_
#define TREE_ITERATOR_

#include "node.hpp"

namespace yLAB {

namespace detail {

template<typename KeyT>
class TreeIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = Node<KeyT>;
    using pointer           = value_type*;
    using reference         = value_type&;
    using const_pointer     = const Node<KeyT>*;
    using const_reference   = const Node<KeyT>&;
    using difference_type   = int;

    TreeIterator(pointer ptr = nullptr) noexcept
        : ptr_ {ptr} {}

    TreeIterator(const TreeIterator& rhs) = default;

    ~TreeIterator() = default;

    const_pointer get_pointer() const noexcept { return ptr_; }

    TreeIterator& operator++() noexcept{
        if (ptr_ == nullptr) {
            ptr_ = nullptr;
            return *this;
        } else {
            if (ptr_->right_) {
                go_to_most_left();
            } else {
                go_upper_inc();
            }
        }
        return *this;
    }

    TreeIterator& operator--() noexcept {
        if (ptr_ == nullptr) {
            ptr_ = nullptr;
            return *this;
        } else {
            if (ptr_->left_) {
                go_to_most_right();
            } else {
                go_upper_dec();
            }
        }
        return *this;
    }

    TreeIterator operator++(int n) noexcept {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeIterator operator--(int n) noexcept {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    KeyT operator*() const noexcept { return ptr_->key_; }
    const_pointer operator->() const noexcept { return ptr_; }
    KeyT& operator*() noexcept { return ptr_->key_; }
    pointer operator->() noexcept { return ptr_; }

private:

    void go_to_most_left() {
        ptr_ = ptr_->right_;
        while(ptr_->left_) {
            ptr_ = ptr_->left_;
        }
    }

    void go_to_most_right() {
        ptr_ = ptr_->left_;
        while(ptr_->right_) {
            ptr_ = ptr_->right_;
        }
    }

    void go_upper_dec() {
        auto tmp = ptr_->parent_;
        while(ptr_ == tmp->left_) {
            ptr_ = std::exchange(tmp, tmp->parent_);
        }
        if (ptr_->left_ != tmp) {
            ptr_ = tmp;
        }
        std::cout << "KEY = " << ptr_->key_ << std::endl;
    }

    void go_upper_inc() {
        auto tmp = ptr_->parent_;
        while(ptr_ == tmp->right_) {
            ptr_ = std::exchange(tmp, tmp->parent_);
        }
        if (ptr_->right_ != tmp) {
            ptr_ = tmp;
        }
        std::cout << "KEY = " << ptr_->key_ << std::endl;
    }
/*------------------------------------------------------------------*/
    template<typename T, typename Comporator>
    friend class AVL_Tree;

    pointer ptr_;
}; // <--- class TreeIterator

template<typename KeyT>
bool operator==(TreeIterator<KeyT> lhs, TreeIterator<KeyT> rhs) noexcept {
    return lhs.get_pointer() == rhs.get_pointer();
}

} // <--- namespace detail

} // <--- namespace yLAB

#endif
