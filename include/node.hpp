#ifndef TREE_NODE
#define TREE_NODE

namespace yLAB {

template<typename KeyT>
struct Node {
    Node(KeyT key, Node* parent = nullptr)
    : key_ {key},
      parent_ {parent} {};

    ~Node() = default;
/*--------------------------------------------------------------*/
    KeyT key_;
    Node* parent_, *left_{nullptr}, *right_{nullptr};
    int height_ {0};
};

} // <--- namespace graphics

#endif

