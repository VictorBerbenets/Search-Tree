#ifndef TREE_ITERATOR_
#define TREE_ITERATOR_

#include <iterator>
#include <memory>
#include <cstddef>

#include "node.hpp"

namespace yLAB {

namespace detail {

template<typename KeyT>
class TreeIterator final {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = Node<KeyT>;
    using pointer           = value_type*;
    using reference         = value_type&;
    using const_pointer     = const value_type*;
    using const_reference   = const value_type&;
    using difference_type   = std::ptrdiff_t;

    TreeIterator(const_pointer ptr) noexcept
    : ptr_ {ptr} {}

    const_pointer get_pointer() const noexcept { return ptr_; }

    TreeIterator& operator++() {
        ptr_ = ptr_->successor();
        return *this;
    }

    TreeIterator& operator--() {
        ptr_ = ptr_->predecessor();
        return *this;
    }

    TreeIterator operator++(int n) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeIterator operator--(int n) {
        auto tmp = *this;
        --(*this);
        return tmp;
    }

    const KeyT& operator*() const noexcept { return ptr_->key_; }
    const KeyT* operator->() const noexcept { return std::addressof(ptr_->key_); }
private:
/*------------------------------------------------------------------*/
    const_pointer ptr_;
}; // <--- class TreeIterator

template<typename KeyT>
bool operator==(TreeIterator<KeyT> lhs, TreeIterator<KeyT> rhs) noexcept {
    return lhs.get_pointer() == rhs.get_pointer();
}

template<typename KeyT>
bool operator!=(TreeIterator<KeyT> lhs, TreeIterator<KeyT> rhs) noexcept {
    return !(lhs == rhs);
}

template<typename KeyT>
bool operator<(TreeIterator<KeyT> lhs, TreeIterator<KeyT> rhs) noexcept {
    return lhs.get_pointer()->key_ < rhs.get_pointer()->key_;
}

} // <--- namespace detail

} // <--- namespace yLAB

#endif
