#ifndef TREE_NODE
#define TREE_NODE

#include <utility>
#include <algorithm>
#include <cstddef>

namespace yLAB {

namespace detail {

template<typename KeyT>
struct Node {
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = Node*;
    using const_pointer   = const Node*;

    Node(const KeyT& key, pointer parent = nullptr)
    : key_ {key},
      parent_ {parent} {};

    virtual ~Node() {};

    pointer predecessor() const { /*predecessor - the node that lies just behind the given node*/
        if (left_) {
            return get_most_right(left_);
        }
        return const_cast<pointer>(go_upper_dec());
    }

    pointer successor() const { /*successor - the node that lies just after the given node*/
        if (right_) {
            return get_most_left(right_);
        }
        return const_cast<pointer>(go_upper_inc());
    }

    static auto get_most_right(pointer start) noexcept {
        while(start->right_) {
            start = start->right_;
        }
        return start;
    }

    static auto get_most_left(pointer start) noexcept {
        while(start->left_) {
            start = start->left_;
        }
        return start;
    }

    static difference_type height_difference(const_pointer left, const_pointer right) noexcept {
        return (left ? (left->height_ + 1) : 0) - (right ? (right->height_ + 1) : 0);
    }

    size_type node_size() const noexcept {
        if (!left_ && !right_) {
            return 1;
        }
        size_type left_s  = size(left_);
        size_type right_s = size(right_);
        return left_s + right_s + 1;
    }

    size_type node_height() const noexcept {
        if (!left_ && !right_) {
            return 0;
        }
        size_type left_h  = height(left_);
        size_type right_h = height(right_);
        return std::max(left_h, right_h) + 1;
    }

private:
    auto go_upper_dec() const {
        auto tmp = parent_;
        auto copy = this;
        while(copy == tmp->left_) {
            copy = std::exchange(tmp, tmp->parent_);
        }
        if (copy->left_ != tmp) {
            copy = tmp;
        }
        return copy;
    }

    auto go_upper_inc() const {
        auto tmp = parent_;
        auto copy = this;
        while(copy == tmp->right_) {
            copy = std::exchange(tmp, tmp->parent_);
        }
        if (copy->right_ != tmp) {
            copy = tmp;
        }
        return copy;
    }

    static size_type height(pointer pt) noexcept {
        return pt ? pt->height_ : 0;
    }

    static size_type size(pointer pt) noexcept {
        return pt ? pt->size_ : 0;
    }
public:
    KeyT key_;
    pointer parent_, left_{nullptr}, right_{nullptr};
    int height_ {0};
    std::size_t size_ {1}; // size is needed for main task: implement fast distance method with O(log(n)) complexity
};

template<typename KeyT>
struct EndNode final: public Node<KeyT> {
    using typename Node<KeyT>::pointer;

    explicit EndNode(pointer root = nullptr)
    : Node<KeyT>{KeyT(), root} {}
};

} // <--- namespace detail

} // <--- namespace graphics

#endif

