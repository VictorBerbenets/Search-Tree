#ifndef TREE_NODE
#define TREE_NODE

#include <utility>

namespace yLAB {

namespace detail {

template<typename KeyT>
struct Node {
    using pointer       = Node*;
    using const_pointer = const Node*;

    Node(const KeyT& key, pointer parent = nullptr)
    : key_ {key},
      parent_ {parent} {};

    const_pointer predecessor() const { /*predecessor - the node that lies just behind the given node*/
        if (left_) {
            return get_most_right();
        }
        return go_upper_dec();
    }

    const_pointer successor() const { /*successor - the node that lies just after the given node*/
        if (right_) {
            return get_most_left();
        }
        return go_upper_inc();
    }

    auto get_most_left() const {
        auto tmp = right_;
        while(tmp->left_) {
            tmp = tmp->left_;
        }
        return tmp;
    }

    auto get_most_right() const {
        auto tmp = left_;
        while(tmp->right_) {
            tmp = tmp->right_;
        }
        return tmp;
    }

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

    KeyT key_;
    pointer parent_, left_{nullptr}, right_{nullptr};
    int height_ {0};
};

template<typename KeyT>
struct EndNode: public Node<KeyT> {
    explicit EndNode(Node<KeyT>::pointer root = nullptr)
    : Node<KeyT>{0, root} {}

};

} // <--- namespace detail

} // <--- namespace graphics

#endif

