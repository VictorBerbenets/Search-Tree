#include <gtest/gtest.h>
#include <set>

#include "search_tree.hpp"

using namespace yLAB;
using def_type = int;

TEST(Lower_Upper_Methods, Lower1) {
    AVL_Tree<def_type> avl{1};
    std::set<def_type> set{1};

    ASSERT_EQ(*avl.lower_bound(1), *set.lower_bound(1));
}

TEST(Lower_Upper_Methods, Lower2) {
    AVL_Tree<def_type> avl{};

    ASSERT_EQ(avl.lower_bound(10543532), avl.cend());
}

TEST(Lower_Upper_Methods, Lower3) {
    std::set<def_type> avl{{1, 3, 4, 5, -1, -2, -3, 0, 56}};
    std::set<def_type> set{{1, 3, 4, 5, -1, -2, -3, 0, 56}};

    for (auto val : avl) {
        ASSERT_EQ(*avl.lower_bound(val), *set.lower_bound(val));
    }
}

TEST(Lower_Upper_Methods, Lower4) {
    std::set<def_type> avl{};
    std::set<def_type> set{};
    for (def_type i = 0; i < 100000; ++i) {
        avl.insert(i);
        set.insert(i);
    }

    for (def_type i = 0; i < 100000; ++i) {
        ASSERT_EQ(*avl.lower_bound(i), *set.lower_bound(i));
    }
}

TEST(Lower_Upper_Methods, Lower5) {
    AVL_Tree<def_type> avl{-100};

    ASSERT_EQ(*avl.lower_bound(-101), -100);
}

TEST(Lower_Upper_Methods, Upper1) {
    AVL_Tree<def_type> avl{1};

    ASSERT_EQ(avl.upper_bound(1), avl.cend());
}

TEST(Lower_Upper_Methods, Upper2) {
    AVL_Tree<def_type> avl{};

    ASSERT_EQ(avl.upper_bound(10543532), avl.cend());
}

TEST(Lower_Upper_Methods, Upper3) {
    std::set<def_type> avl{{1, 3, 4, 5, -1, -2, -3, 0, 56}};
    std::set<def_type> set{{1, 3, 4, 5, -1, -2, -3, 0, 56}};

    for (auto val : avl) {
        ASSERT_EQ(*avl.upper_bound(val), *set.upper_bound(val));
    }
}

TEST(Lower_Upper_Methods, Upper4) {
    std::set<def_type> avl{};
    std::set<def_type> set{};
    for (def_type i = 0; i < 100000; ++i) {
        avl.insert(i);
        set.insert(i);
    }

    for (def_type i = 0; i < 100000; ++i) {
        ASSERT_EQ(*avl.upper_bound(i), *set.upper_bound(i));
    }
}

TEST(Lower_Upper_Methods, Upper5) {
    AVL_Tree<def_type> avl{-100};

    ASSERT_EQ(*avl.upper_bound(-101), -100);
}