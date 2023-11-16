#ifndef SEARCH_TREE
#define SEARCH_TREE

#include <iostream>
#include <utility>
#include <iterator>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <queue>
#include <cstddef>

#include "tree_iterator.hpp"
#include "node.hpp"
#include "graph_tree.hpp"

namespace yLAB {

template<typename KeyT = int, typename Compare = std::less<KeyT>>
class AVL_Tree final {
public:
    using key_type               = KeyT;
    using value_type             = KeyT;
    using size_type              = std::size_t;
    using key_compare            = Compare;
    using value_compare          = Compare;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using node_type              = detail::Node<key_type>;
    using difference_type        = node_type::difference_type;
    using pointer                = node_type*;
    using const_pointer          = const node_type*;
    using const_iterator         = TreeIterator<key_type>;
    using iterator               = const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    using end_node    = detail::EndNode<key_type>;
    using end_pointer = end_node*;

    enum class childPosition : char {Left, Right};

    static constexpr difference_type DIFF_HEIGHT = 2; // difference between two subtree heights
public:
    template<std::input_iterator InputIter>
    AVL_Tree(InputIter begin, InputIter end, const Compare& comp = Compare())
    : comp_ {comp} {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    AVL_Tree(std::initializer_list<KeyT> ls, const Compare& comp = Compare())
    : AVL_Tree {ls.begin(), ls.end(), comp} {}

    explicit AVL_Tree(const Compare& comp = Compare())
    : comp_ {comp} {}

    AVL_Tree(const AVL_Tree& rhs)
    : size_ {rhs.size_},
      comp_ {rhs.comp_} {
        if (!rhs.size_) { return ; }

        create_root_node(rhs.root_node_->key_);

        std::queue<pointer> rhs_nodes {}; // level tree traversal nodes
        std::queue<pointer> nodes     {}; // creating nodes

        rhs_nodes.push(rhs.root_node_);
        nodes.push(root_node_);
        while(rhs_nodes.size()) {
            auto parent   = nodes.front();
            auto rhs_node = rhs_nodes.front();
            if (rhs_node->left_) {
                parent->left_ = new node_type{rhs_node->left_->key_, parent};
                rhs_nodes.push(rhs_node->left_);
                nodes.push(parent->left_);
            }
            if (rhs_node->right_) {
                parent->right_ = new node_type{rhs_node->right_->key_, parent};
                rhs_nodes.push(rhs_node->right_);
                nodes.push(parent->right_);
            }
            rhs_nodes.pop();
            nodes.pop();
        }
        begin_node_ = node_type::get_most_left(root_node_);
    }

    AVL_Tree(AVL_Tree&& rhs)
    : root_node_  {std::exchange(rhs.root_node_, nullptr)},
      begin_node_ {std::exchange(rhs.begin_node_, rhs.end_ptr_)},
      size_       {std::exchange(rhs.size_, 0)},
      comp_       {rhs.comp_} {
        rebalance_tree_pointers();
    }

    ~AVL_Tree() { clear_tree(); }

    AVL_Tree& operator=(const AVL_Tree& rhs) {
        if (this == std::addressof(rhs)) {
            return *this;
        }
        auto copy = rhs;
        swap(copy);
        return *this;
    }

    AVL_Tree& operator=(AVL_Tree&& rhs) noexcept(noexcept(swap(rhs))) {
        swap(rhs);
        return *this;
    }

    void swap(AVL_Tree& rhs) noexcept(std::is_nothrow_swappable_v<Compare>) {
        std::swap(root_node_, rhs.root_node_);
        std::swap(begin_node_, rhs.begin_node_);
        std::swap(comp_, rhs.comp_);
        std::swap(size_, rhs.size_);

        rebalance_tree_pointers();
        rhs.rebalance_tree_pointers();
    }

    const_iterator find(const key_type& key) const noexcept(noexcept(find_in_subtree(root_node_, key))) {
        return find_in_subtree(root_node_, key);
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return insert( key_t(std::forward<Args>(args)...) );
    }

    void insert(std::initializer_list<value_type> ilist) {
        for(auto&& val : ilist) {
            insert(val);
        }
    }

    std::pair<iterator, bool> insert(const key_type& key) {
        if (root_node_ == nullptr) {
            ++size_;
            return {create_root_node(key), true};
        }

        auto curr_node = root_node_;
        while(curr_node) {
            if (comp_(key, curr_node->key_)) {
                if (!curr_node->left_) {
                    create_node(curr_node, key, childPosition::Left);
                    break;
                }
                curr_node = curr_node->left_;
            } else if (comp_(curr_node->key_, key)) {
                if (!curr_node->right_) {
                    create_node(curr_node, key, childPosition::Right);
                    break;
                }
                curr_node = curr_node->right_;
            } else {
                return {construct_iterator(curr_node), false};
            }
        }

        balance_tree(curr_node);
        begin_node_ = node_type::get_most_left(begin_node_);

        return {construct_iterator(curr_node), true};
    }

    size_type erase(const key_type& key) {
        size_type save_size = size();

        auto erase_it = find(key);
        if (erase_it == end()) {
            return 0;
        }
        erase_node(erase_it);

        return size() != save_size;
    }

    iterator erase(const_iterator erase_it) {
        if (erase_it.ptr_ != find(*erase_it).ptr_) { return std::next(erase_it); }
        return erase_node(erase_it);
    }

    iterator erase(const_iterator begin, const_iterator end) {
        for (; begin != end; ) {
            begin = erase(begin);
        }
        return begin;
    }

    const_iterator lower_bound(const key_type& key) const {
        if (!root_node_) { return cend(); }

        auto curr_node = root_node_;
        pointer result {nullptr};
        while (curr_node) {
            if (!comp_(curr_node->key_, key)) {
                result = curr_node;
                curr_node = curr_node->left_;
            } else {
                curr_node = curr_node->right_;
            }
        }
        return result ? construct_iterator(result) : cend();
    }

    const_iterator upper_bound(const key_type& key) const {
       if (!root_node_) { return cend(); }

        auto curr_node = root_node_;
        pointer result {nullptr};
        while (curr_node) {
            if (comp_(key, curr_node->key_)) {
                result = curr_node;
                curr_node = curr_node->left_;
            } else {
                curr_node = curr_node->right_;
            }
        }
        return result ? construct_iterator(result) : cend();
    }

    int distance(const_iterator begin, const_iterator end) const {
        auto [curr_ptr, end_ptr] = std::make_pair(begin.ptr_, end.ptr_);
        size_type dist {0};
        if (end_ptr == end_ptr_) {
            if (curr_ptr == end_ptr) {
                return 0;
            }
            end_ptr = node_type::get_most_right(root_node_);
            ++dist;
        }

        while(curr_ptr != end_ptr) {
            if (find_in_subtree(curr_ptr, end_ptr->key_) == cend()) {
                curr_ptr = go_up_to_right_parent(curr_ptr, dist); 
            } else {
                curr_ptr = go_down_to_right_node(curr_ptr, end_ptr, dist);
            }
        }
        return dist;
    }

    const_iterator min_account_elem(size_type number) const {
        if (empty() || number == 0) { return cend(); }
        if (number > size_) { return cbegin(); }

        auto curr_ptr = cbegin().ptr_;
        size_type path_len {0};
        for (size_type counter = curr_ptr->size_; counter < number && curr_ptr != root_node_; counter = curr_ptr->size_) {
            path_len = curr_ptr->size_;
            curr_ptr = curr_ptr->parent_;
        }
        auto node_size = [](pointer ptr) {
            return ptr ? ptr->size_ : 0;
        };
        auto full_path = [](size_type sz1, size_type sz2, size_type extra_size) {
            return sz1 + sz2 - extra_size;
        };

        if (curr_ptr->size_ - node_size(curr_ptr->right_) == number) {
            return construct_iterator(curr_ptr);
        }
        ++path_len;
        curr_ptr = curr_ptr->right_;
        while(full_path(curr_ptr->size_, path_len, node_size(curr_ptr->right_)) != number) {
            auto compare_path_length = node_size(curr_ptr->left_) + path_len + 1;
            if (compare_path_length > number) {
                curr_ptr = curr_ptr->left_;
            } else if (compare_path_length < number) {
                path_len += curr_ptr->size_ - curr_ptr->right_->size_;
                curr_ptr = curr_ptr->right_;
            } else { break; }
        }
        return construct_iterator(curr_ptr);
    }

    size_type lower_than_one(const key_type& key) const {
        return distance(cbegin(), lower_bound(key));
    }

    bool contains(const key_type& key) const {
        return find(key) != cend();
    }

    size_type count(const key_type& key) const {
        return contains(key);
    }

    void clear() noexcept {
        clear_tree();
        begin_node_     = end_ptr_;
        root_node_      = nullptr;
        end_ptr_->left_ = nullptr;
        size_           = 0;
    }

    size_type size() const noexcept { return size_; };
    [[nodiscard]] bool empty() const noexcept     { return size_ == 0; };

    void graph_dump(const std::string& file_name) const {
       graphics::tree_painter<KeyT> graph {root_node_};
       graph.graph_dump(file_name);
    }

    iterator begin() const noexcept { return construct_iterator(begin_node_); }
    iterator end()   const noexcept { return construct_iterator(end_ptr_);    }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend()   const noexcept { return end();   }
    reverse_iterator rbegin() const { return std::make_reverse_iterator(end());   }
    reverse_iterator rend()   const { return std::make_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return std::make_reverse_iterator(cend());   }
    const_reverse_iterator crend()   const { return std::make_reverse_iterator(cbegin()); }
private:
    pointer right_turn(pointer pt) {
        set_child_parent_connection(pt, pt->left_);

        auto local_child = std::exchange(pt->left_, pt->left_->right_);

        if (local_child->right_) {
            local_child->right_->parent_ = pt;
        }
        pt->parent_ = local_child;
        local_child->right_ = pt;

        correct_nodes(local_child->right_);
        correct_nodes(local_child->left_);

        return local_child;
    }

    pointer left_turn(pointer pt) {
        set_child_parent_connection(pt, pt->right_);

        auto local_child = std::exchange(pt->right_, pt->right_->left_);

        if (local_child->left_) {
            local_child->left_->parent_ = pt;
        }
        pt->parent_ = local_child;
        local_child->left_ = pt;

        correct_nodes(local_child->left_);
        correct_nodes(local_child->right_);

        return local_child;
    }

    pointer big_right_turn(pointer pt) {
        set_child_parent_connection(pt, pt->left_->right_);

        auto local_root = pt->left_;
        auto local_root_child = local_root->right_;

        connect_two_nodes(pt, local_root_child->right_, childPosition::Left);
        connect_two_nodes(local_root, local_root_child->left_, childPosition::Right);

        local_root_child->right_ = pt;
        local_root_child->left_  = local_root;

        pt->parent_ = local_root->parent_ = local_root_child;

        set_child_height(local_root_child->right_);
        correct_nodes(local_root_child->left_);
        correct_nodes(local_root_child->right_);

        return local_root_child;
    }

    pointer big_left_turn(pointer pt) {
        set_child_parent_connection(pt, pt->right_->left_);

        auto local_root = pt->right_;
        auto local_root_child = local_root->left_;

        connect_two_nodes(pt, local_root_child->left_, childPosition::Right);
        connect_two_nodes(local_root, local_root_child->right_, childPosition::Left);

        local_root_child->left_  = pt;
        local_root_child->right_ = local_root;

        pt->parent_ = local_root->parent_ = local_root_child;

        set_child_height(local_root_child->right_);
        correct_nodes(local_root_child->left_);
        correct_nodes(local_root_child->right_);

        return local_root_child;
    }

    void set_child_height(pointer child) {
        child->height_ = is_child(child) ? 0 : child->node_height();
    }

    void connect_two_nodes(pointer first_node, pointer second_node, childPosition pos) {
        if(pos == childPosition::Left) {
            first_node->left_ = second_node;
        } else {
            first_node->right_ = second_node;
        }
        if (second_node) {
            second_node->parent_ = first_node;
        }
    }

    void set_child_parent_connection(pointer pt, pointer child) {
        if (auto parent = pt->parent_; parent != end_ptr_) {
            (parent->left_ == pt ? parent->left_ : parent->right_) = child;
            if (child) {
                child->parent_ = parent;
            }
        } else {
            root_node_ = child;
            if (root_node_) {
                root_node_->parent_ = end_ptr_;
            }
            end_ptr_->left_ = root_node_;
        }
    }

    void correct_nodes(pointer pt) {
        while(pt != end_ptr_) {
            pt->height_ = pt->node_height();
            pt->size_   = pt->node_size();
            pt = pt->parent_;
        }
    }

    void balance_tree(pointer pt) {
        while(pt != end_ptr_) {
            if (is_disbalance(node_type::height_difference(pt->right_, pt->left_))) {
                if (node_type::height_difference(pt->right_->right_, pt->right_->left_) >= 0) {
                    pt = left_turn(pt);
                } else {
                    pt = big_left_turn(pt);
                }
            } else if (is_disbalance(node_type::height_difference(pt->left_, pt->right_))) {
                if (node_type::height_difference(pt->left_->left_, pt->left_->right_) >= 0) {
                    pt = right_turn(pt);
                } else {
                    pt = big_right_turn(pt);
                }
            }
            pt = pt->parent_;
        }
    }

    bool is_disbalance(difference_type diff) const noexcept { return diff == DIFF_HEIGHT; }

    const_iterator find_in_subtree(pointer start_find, const key_type& key) const noexcept(noexcept(comp_(key, key))) {
        while(start_find) {
            if (comp_(key, start_find->key_)) {
                start_find = start_find->left_;
            } else if (comp_(start_find->key_, key)) {
                start_find = start_find->right_;
            } else {
                return construct_iterator(start_find);
            }
        }
        return cend();
    }

    iterator erase_node(iterator erase_it) {
        if (erase_it == end()) { return std::next(erase_it); }

        auto erase_ptr   = erase_it.ptr_;
        auto replace_ptr = erase_ptr->left_ ? node_type::get_most_right(erase_ptr->left_) : nullptr;
        pointer start_balance{nullptr};
        iterator next_iter = std::next(erase_it);
        if (replace_ptr) {
            if (replace_ptr->parent_ != erase_ptr) {
                replace_ptr->parent_->right_ = nullptr;
            }
            set_child_parent_connection(erase_ptr, replace_ptr);

            if (erase_ptr->left_ != replace_ptr) {
                auto most_replace_left = node_type::get_most_left(replace_ptr);
                start_balance          = most_replace_left;
                connect_two_nodes(most_replace_left, erase_ptr->left_, childPosition::Left);
            } else {
                start_balance = replace_ptr;
            }
            connect_two_nodes(replace_ptr, erase_ptr->right_, childPosition::Right);
        } else {
            start_balance = erase_ptr->parent_;
            set_child_parent_connection(erase_ptr, erase_ptr->right_);
        }
        delete erase_ptr;
        correct_nodes(start_balance);
        balance_tree(start_balance);
        begin_node_ = node_type::get_most_left(end_ptr_);
        --size_;

        return next_iter;
    }

    void rebalance_tree_pointers() noexcept {
        if (root_node_) {
            root_node_->parent_ = end_ptr_;
            end_ptr_->left_     = root_node_;
        } else {
            begin_node_     = end_ptr_;
            end_ptr_->left_ = nullptr;
        }
    }

    iterator create_root_node(const key_type& key) {
        root_node_ = new node_type{key};
        end_node_.left_ = root_node_;
        root_node_->parent_ = end_ptr_;
        begin_node_ = root_node_;

        return construct_iterator(root_node_);
    }

    void create_node(pointer curr_node, const key_type& key, childPosition pos) {
        if (pos == childPosition::Left) {
            curr_node->left_ = new node_type{key, curr_node};
        } else {
            curr_node->right_ = new node_type{key, curr_node};
        }
        correct_nodes(curr_node);
        ++size_;
    }

    static bool is_child(pointer pt) noexcept {
        return pt->left_ == nullptr && pt->right_ == nullptr;
    }

    void clear_tree() noexcept {
        for (pointer curr_node = root_node_, tmp {nullptr}; curr_node; curr_node = tmp) {
            if (!curr_node->left_) {
                tmp = curr_node->right_;
                delete curr_node;
            } else {
                tmp = curr_node->left_;
                curr_node->left_ = tmp->right_;
                tmp->right_ = curr_node;
            }
        }
    }

    pointer go_down_to_right_node(pointer curr_ptr, pointer end_ptr, size_type& dist) const noexcept(noexcept(comp_(key_type{}, key_type{}))){
        curr_ptr = curr_ptr->right_;
        ++dist;
        while(curr_ptr != end_ptr) {
            if (comp_(end_ptr->key_, curr_ptr->key_)) {
                curr_ptr = curr_ptr->left_;
            } else if (comp_(curr_ptr->key_, end_ptr->key_)) {
                dist += curr_ptr->size_ - curr_ptr->right_->size_;
                curr_ptr = curr_ptr->right_;
            }
        }
        if (curr_ptr->left_) {
            dist += curr_ptr->left_->size_;
        }
        return curr_ptr;
    }

    pointer go_up_to_right_parent(pointer curr_ptr, size_type& dist) const noexcept {
        auto child = curr_ptr->left_;
        if (curr_ptr->left_ == child) {
            if (curr_ptr->right_) {
                dist += curr_ptr->right_->size_;
            }
        }
        child     = curr_ptr;
        curr_ptr = curr_ptr->parent_;
        while (curr_ptr->right_ == child) {
            child     = curr_ptr;
            curr_ptr = curr_ptr->parent_;
        }
        ++dist;
        return curr_ptr;
    }

    iterator construct_iterator(pointer ptr) const noexcept {
        iterator tmp;
        tmp.ptr_ = ptr;
        return tmp;
    }

private:
    pointer root_node_ {};
    end_node end_node_ {};
    end_pointer end_ptr_ {std::addressof(end_node_)};
    pointer begin_node_ {end_ptr_};
    size_type size_ {0};
    Compare comp_;
}; // <--- class AVL_Tree

template<typename KeyT, typename Compare>
std::ostream& operator<<(std::ostream& os, const AVL_Tree<KeyT, Compare>& rhs) {
    for (const auto&& val : rhs) {
        os << val << ' ';
    }
    return os << std::endl;
}

template<typename KeyT, typename Compare>
bool operator==(const AVL_Tree<KeyT, Compare>& lhs, const AVL_Tree<KeyT, Compare>& rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

} // <--- namespace yLAB

#endif
