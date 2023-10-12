#include <gtest/gtest.h>
#include <iterator>
#include <set>

#include "search_tree.hpp"

using namespace yLAB;

TEST(DistanceMethod, Distance1) {
    AVL_Tree<int> avl{};
    auto it1 = avl.begin();
    auto it2 = avl.end();

    ASSERT_EQ(avl.distance(it1, it2), std::distance(it1, it2));
}

TEST(DistanceMethod, Distance2) {
    AVL_Tree<int> avl{1};
    auto it1 = avl.begin();
    auto it2 = avl.end();

    ASSERT_EQ(avl.distance(it1, it2), std::distance(it1, it2));
}

TEST(DistanceMethod, Distance3) {
    AVL_Tree<int> avl{1, 2, 3, 4, 5, -1, -2, -3, -4, -5, 0};
    auto it1 = avl.begin();
    auto it2 = avl.end();

    ASSERT_EQ(avl.distance(it1, it2), std::distance(it1, it2));
}

TEST(DistanceMethod, Distance4) {
    AVL_Tree<int> avl{};
    constexpr int size = 100;
    for (auto i = 0; i < size; ++i) {
        avl.emplace(i);
    }

    for (int i = 0; i < size - 1; ++i) {
        auto it1 = avl.find(i);
        for (int j = i; j < size - 1; ++j) {
            auto it2 = avl.find(j);
            ASSERT_EQ(avl.distance(it1, it2), std::distance(it1, it2));
        }
    }
}

TEST(DistanceMethod, Distance5) {
    AVL_Tree<int> avl{};
    constexpr int size = 1000000;
    for (auto i = 0; i < size; ++i) {
        avl.emplace(i);
    }

    ASSERT_EQ(avl.distance(avl.begin(), avl.end()), size);
}
