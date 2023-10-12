#ifndef SEARCH_TREE
#define SEARCH_TREE

#include <iostream>
#include <utility>
#include <tuple>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <queue>
#include <cstddef>
#include <cassert>

#include "tree_iterator.hpp"
#include "node.hpp"
#include "graph_tree.hpp"

namespace yLAB {

template<typename KeyT = int, typename Compare = std::less<KeyT>>
class AVL_Tree final {
public:
    using key_type        = KeyT;
    using value_type      = KeyT;
    using size_type       = std::size_t;
    using key_compare     = Compare;
    using value_compare   = Compare;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using node_type       = detail::Node<key_type>;
    using difference_type = node_type::difference_type;
    using pointer         = node_type*;
    using const_pointer   = const node_type*;
    using iterator        = detail::TreeIterator<key_type>;
    using const_iterator  = iterator;
private:
    using end_node    = detail::EndNode<key_type>;
    using end_pointer = end_node*;

    enum class childPosition : char {Left = 1, Right = 2};

    static constexpr difference_type DIFF_HEIGHT = 2; // difference between two subtree heights
public:
    template <typename Iter>
    AVL_Tree(Iter begin, Iter end, const Compare& comp = Compare())
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

    const_iterator find(const key_type& key) const noexcept(noexcept(find_in_subtree(root_node_, key))){
        return find_in_subtree(root_node_, key);
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args... args) {
        key_type key(std::forward<Args>(args)...);
        return insert(key);
    }

    void insert(std::initializer_list<value_type> ilist) {
        for(auto il_it = ilist.begin(); il_it != ilist.end(); ++il_it) {
            insert(*il_it);
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
                return {iterator{curr_node}, false};
            }
        }

        balance_tree(curr_node);
        begin_node_ = node_type::get_most_left(begin_node_);

        return {iterator{curr_node}, true};
    }

    size_type erase(const key_type& key) {
        size_type save_size = size();
        node_type tmp_node{key};
        erase(const_iterator{&tmp_node});
        return size() != save_size;
    }

    iterator erase(const_iterator pos) {
        auto erase_it = find(*pos);
        if (pos == cend() || erase_it == cend()) { return end(); }

        auto erase_ptr   = const_cast<pointer>(erase_it.get_pointer());
        auto replace_ptr = erase_ptr->left_ ? node_type::get_most_right(erase_ptr->left_) : nullptr;
        pointer start_balance{nullptr};
        iterator next_iter = ++iterator{erase_ptr};
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

    iterator erase(const_iterator begin, const_iterator end) {
        iterator tmp{nullptr};
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
        return result ? result : cend();
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
        return result ? result : cend();
    }

    int distance(const_iterator begin, const_iterator end) const {
        using iters_order = std::tuple<const_pointer, const_pointer, int>;

        auto [start_ptr, end_ptr, is_direct] = begin < end ?
        iters_order(begin.get_pointer(), end.get_pointer(), 1) : iters_order(end.get_pointer(), begin.get_pointer(), -1);
        int dist {0};
        const_pointer child = start_ptr->left_;
        while(start_ptr != end_ptr && start_ptr) {
#if 0
            if (start_ptr->right_ == child) {
                std::cout << "IN THE BEGIN\n";
                child     = start_ptr;
                start_ptr = start_ptr->parent_;
                ++dist;
                continue;
            }
            std::cout << "KEY  = " << start_ptr->key_ << std::endl;
            std::cout << "DIST = " << dist << std::endl;
#endif
            if (find_in_subtree(start_ptr, end_ptr->key_) == cend()) {
#if 0
                std::cout << "IN IF\n";
                std::cout << "CHILD = " << child->key_ << std::endl;
#endif
                if (start_ptr->left_ == child) {
                    if (start_ptr->right_) {
                        dist += start_ptr->right_->size_;
                    }
                }
                child     = start_ptr;
                start_ptr = start_ptr->parent_;
                ++dist;
                if (start_ptr->right_ == child) {
                    child     = start_ptr;
                    start_ptr = start_ptr->parent_;
                    continue;
                }

            } else {
                start_ptr = start_ptr->right_;
                ++dist;
#if 0
                std::cout << "IN ELSE\n";
                std::cout << "KEY  = " << start_ptr->key_ << std::endl;
                std::cout << "DIST = " << dist << std::endl;
#endif
                while(start_ptr != end_ptr && start_ptr) {
                    if (comp_(end_ptr->key_, start_ptr->key_)) {
                        start_ptr = start_ptr->left_;
                    } else if (comp_(start_ptr->key_, end_ptr->key_)) {
                        assert(start_ptr->right_);
                        dist += start_ptr->size_ - start_ptr->right_->size_;
                        start_ptr = start_ptr->right_;
                    }
                }
                if (start_ptr->left_) {
                    dist += start_ptr->left_->size_;
                }
            }
        }
        return dist * is_direct;
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
    bool empty() const noexcept     { return size_ == 0; };

    void graph_dump(const std::string& file_name = "graph.png") const {
       graphics::tree_painter<KeyT> graph {end_ptr_};
       graph.graph_dump(file_name);
    }

    const_iterator begin()  const noexcept { return const_iterator{begin_node_}; }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator end()    const noexcept { return const_iterator{end_ptr_}; }
    const_iterator cend()   const noexcept { return end(); }
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
        auto parent = pt->parent_;

        if (parent != end_ptr_) {
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

    const_iterator find_in_subtree(const_pointer start_find, const key_type& key) const noexcept(noexcept(comp_(key, key))) {
        while(start_find) {
            if (comp_(key, start_find->key_)) {
                start_find = start_find->left_;
            } else if (comp_(start_find->key_, key)) {
                start_find = start_find->right_;
            } else {
                return const_iterator{start_find};
            }
        }
        return cend();
    }

    void erase_node(pointer& erase_ptr, pointer child) { // erase_ptr doesn't have at least one child
        auto erase_parent = erase_ptr->parent_;
        if (child) {
            (erase_parent->left_ == erase_ptr ? erase_parent->left_ : erase_parent->right_) = child;
            child->parent_ = erase_parent;
        } else {
            (erase_parent->left_ == erase_ptr ? erase_parent->left_ : erase_parent->right_) = nullptr;
        }
        if (erase_ptr == root_node_) {
            root_node_ = child;
        }
        delete erase_ptr;
        correct_heights(erase_parent);
        balance_tree(erase_parent);
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

        return iterator{root_node_};
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
        for (pointer curr_node = root_node_, tmp{0}; curr_node; curr_node = tmp) {
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
    for (auto val : rhs) {
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
