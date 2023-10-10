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

    virtual ~Node() {};

    const_pointer predecessor() const { /*predecessor - the node that lies just behind the given node*/
        if (left_) {
            return get_most_right(left_);
        }
        return go_upper_dec();
    }

    const_pointer successor() const { /*successor - the node that lies just after the given node*/
        if (right_) {
            return get_most_left(right_);
        }
        return go_upper_inc();
    }

    static auto get_most_right(pointer start) {
        while(start->right_) {
            start = start->right_;
        }
        return start;
    }

    static auto get_most_left(pointer start) {
        while(start->left_) {
            start = start->left_;
        }
        return start;
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
        assert(parent_);
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
public:
    KeyT key_;
    pointer parent_, left_{nullptr}, right_{nullptr};
    int height_ {0};
};

template<typename KeyT>
struct EndNode: public Node<KeyT> {
    explicit EndNode(Node<KeyT>::pointer root = nullptr)
    : Node<KeyT>{KeyT(), root} {}
};

} // <--- namespace detail

} // <--- namespace graphics

#endif

