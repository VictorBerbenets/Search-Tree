#ifndef JUST_COOL_NODE_
#define JUST_COOL_NODE_

#include "node.hpp"

namespace yLAB {

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

    TreeIterator operator++(int n) noexcept{
        if (ptr_->right_) {
            ptr_ = ptr_->right_;
        } else if (ptr_->left_) {
            ptr_ = ptr_->left__;
        }
        } else {
            ptr_ = ptr_parent_;
        }

        return *this;
    }
    TreeIterator operator--(int n) noexcept { ptr_--; return *this; }
    TreeIterator& operator++() noexcept { ++ptr_; return *this; }
    TreeIterator& operator--() noexcept { --ptr_; return *this; }

    const_reference operator*() const noexcept { return *ptr_; }
    const_pointer operator->() const noexcept { return ptr_; }
    reference operator*() noexcept { return *ptr_; }
    pointer operator->() noexcept { return ptr_; }


private:
    pointer ptr_;
}; // <--- class TreeIterator

template<typename KeyT>
bool operator==(TreeIterator<KeyT> lhs, TreeIterator<KeyT> rhs) noexcept {
    return lhs.get_pointer() == rhs.get_pointer();
}

} // <--- namespace yLAB

#endif
