#pragma once

#include <iterator>
#include <memory>
#include <cstddef>

#include "node.hpp"

namespace yLAB {

template<typename KeyT>
class TreeIterator final {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = detail::Node<KeyT>;
    using pointer           = value_type*;
    using reference         = value_type&;
    using const_pointer     = const value_type*;
    using const_reference   = const value_type&;
    using difference_type   = std::ptrdiff_t;

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

    bool operator==(TreeIterator rhs) const noexcept { return ptr_ == rhs.ptr_; }
    bool operator!=(TreeIterator rhs) const noexcept { return !(*this == rhs); }

    template<typename Key, typename Compare> friend class AVL_Tree;
private:
/*----------------------------------------------------------------------------------*/
    pointer ptr_ {nullptr};
};

} // <--- namespace yLAB

