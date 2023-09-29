#ifndef JUST_COOL_NODE_
#define JUST_COOL_NODE_

namespace yLAB {

template<typename T>
class tree_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using const_pointer     = const T*;
    using const_reference   = const T&;
    using difference_type   = int;
public:
    
private:
    pointer ptr_;
};
} // <--- namespace yLAB

#endif
