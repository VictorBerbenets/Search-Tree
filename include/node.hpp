#ifndef TREE_NODE
#define TREE_NODE

#include <cassert>
#include <utility>

namespace yLAB {

namespace detail {

template<typename KeyT>
struct Node {
    using pointer       = Node*;
    using const_pointer = const Node*;

    Node(KeyT key, pointer parent = nullptr)
    : key_ {key},
      parent_ {parent} {};
    
    const_pointer predicessor() const { /*predecessor - the node that lies just behind the given node*/
        auto copy = this;
        if (copy->left_) {
            copy = go_to_most_right(copy);
        } else {
            copy = go_upper_dec(copy);
        }
        return copy;
    }

    const_pointer successor() const { /*successor - the node that lies just after the given node*/
        auto copy = this;
        if (copy->right_) {
            copy = go_to_most_left(copy);
        } else {
            copy = go_upper_inc(copy);
        }
        return copy;
    }

private:    
    auto go_to_most_left(const_pointer copy) const {
        copy = copy->right_;
        while(copy->left_) {
            copy = copy->left_;
        }
        return copy;
    }

    auto go_to_most_right(const_pointer copy) const {
        copy = copy->left_;
        while(copy->right_) {
            copy = copy->right_;
        }
        return copy;
    }

    auto go_upper_dec(const_pointer copy) const {
        assert(copy);
        auto tmp = copy->parent_;
        while(copy == tmp->left_) {
            copy = std::exchange(tmp, tmp->parent_);
        }
        if (copy->left_ != tmp) {
            copy = tmp;
        }
        return copy;
    }

    auto go_upper_inc(const_pointer copy) const {
        assert(copy);
        auto tmp = copy->parent_;
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
    Node* parent_, *left_{nullptr}, *right_{nullptr};
    int height_ {0};
};

template<typename KeyT>
struct EndNode: public Node<KeyT> {
    explicit EndNode(Node<KeyT>* root = nullptr)
    : Node<KeyT>{0, root} {}

};

} // <--- namespace detail

} // <--- namespace graphics

#endif

