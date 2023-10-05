#ifndef TREE_NODE
#define TREE_NODE

namespace yLAB {

namespace detail {

template<typename KeyT>
struct Node {
    Node(KeyT key, Node* parent = nullptr)
    : key_ {key},
      parent_ {parent} {};

/*--------------------------------------------------------------*/
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

