#include <gtest/gtest.h>
#include <set>

#include "search_tree.hpp"

using namespace yLAB;

TEST(Methods, Size1) {
    AVL_Tree<int> avl{};

    ASSERT_EQ(avl.size(), 0);
}

TEST(Methods, Size2) {
    AVL_Tree<int> avl{1};

    ASSERT_EQ(avl.size(), 1);
}

TEST(Methods, Size3) {
    AVL_Tree<int> avl{1};
    constexpr int size = 10000;
    for (int i = 0; i < size; ++i) {
        avl.insert(i);
    }

    ASSERT_EQ(avl.size(), size);
}

TEST(Methods, Empty1) {
    AVL_Tree<int> avl{};

    ASSERT_TRUE(avl.empty());
}

TEST(Methods, Empty2) {
    AVL_Tree<int> avl{1};

    ASSERT_FALSE(avl.empty());
}

TEST(Methods, Empty3) {
    AVL_Tree<int> avl{1};

    ASSERT_FALSE(avl.empty());

    avl.erase(1);

    ASSERT_TRUE(avl.empty());
}

TEST(Methods, Clear1) {
    AVL_Tree<int> avl{};

    ASSERT_TRUE(avl.empty());

    avl.clear();

    ASSERT_TRUE(avl.empty());
}
TEST(Methods, Clear2) {
    AVL_Tree<int> avl{0, 1, 2};

    ASSERT_FALSE(avl.empty());

    avl.clear();

    ASSERT_TRUE(avl.empty());
}

TEST(Methods, Find1) {
    AVL_Tree<int> avl{};

    ASSERT_EQ(avl.find(13245), avl.cend());
}

TEST(Methods, Find2) {
    AVL_Tree<int> avl{1};

    ASSERT_EQ(*avl.find(1), 1);
}

TEST(Methods, Find3) {
    AVL_Tree<int> avl{};
    std::set<int> set{};

    constexpr int size = 10000;
    for (int i = 0; i < size; ++i) {
        avl.insert(i);
        set.insert(i);
    }
    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(*avl.find(i), *set.find(i));
    }
}

TEST(Methods, Erase1) {
    AVL_Tree<int> avl{};

    ASSERT_EQ(avl.erase(13245), 0);
}

TEST(Methods, Erase2) {
    AVL_Tree<int> avl{1};

    ASSERT_EQ(avl.erase(1), 1);
}

TEST(Methods, Erase3) {
    AVL_Tree<int> avl{};
    std::set<int> set{};

    constexpr int size = 10000;
    for (int i = 0; i < size; ++i) {
        avl.insert(i);
        set.insert(i);
    }
    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(avl.erase(i), set.erase(i));
    }
}

TEST(Methods, Swap1) {
    AVL_Tree<int> avl{};
    AVL_Tree<int> tmp = avl;
    AVL_Tree<int> avl2{};

    avl2.swap(avl);

    ASSERT_EQ(avl2, tmp);
}

TEST(Methods, Swap2) {
    AVL_Tree<int> avl{{1, 2, 3, -1, -2, -3, 0}};
    AVL_Tree<int> tmp = avl;
    AVL_Tree<int> avl2{{14888814}};

    avl2.swap(avl);

    ASSERT_EQ(avl2, tmp);
}
