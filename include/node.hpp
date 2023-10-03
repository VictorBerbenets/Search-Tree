#ifndef TREE_NODE
#define TREE_NODE

namespace yLAB {

namespace detail {

template<typename KeyT>
struct Node {
    Node(KeyT key, Node* parent = nullptr)
    : key_ {key},
      parent_ {parent} {};
    
    Node* next_lleft()  const { return left_->left_;   }
    Node* next_lright() const { return left_->right_;  }
    Node* next_rright() const { return right_->right_; }
    Node* next_rleft()  const { return right_->left_; }
/*--------------------------------------------------------------*/
    KeyT key_;
    Node* parent_, *left_{nullptr}, *right_{nullptr};
    int height_ {0};
};

template<typename KeyT>
struct EndNode: private Node<KeyT> {
    explicit EndNode(Node<KeyT>* root)
    : root_node_ {root} {}

    EndNode() = default;

    Node<KeyT>* root_node_;    
};


} // <--- namespace detail

} // <--- namespace graphics

#endif

