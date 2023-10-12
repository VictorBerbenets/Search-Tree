#include <gtest/gtest.h>
#include <utility>
#include <set>

#include "search_tree.hpp"

using namespace yLAB;

TEST(Operators, CopyConstructor1) {
    AVL_Tree<int> avl{};
    AVL_Tree<int> avl2 = avl;

    ASSERT_EQ(avl2, avl);
}

TEST(Operators, CopyConstructor2) {
    AVL_Tree<int> avl{{1, 2, 3, 4, -1, -2, -3, -4, 0}};
    AVL_Tree<int> avl2 = avl;

    ASSERT_EQ(avl2, avl);
}

TEST(Operators, MoveConstructor1) {
    AVL_Tree<int> avl{};
    auto copy = avl;
    AVL_Tree<int> avl2 = std::move(avl);

    ASSERT_EQ(avl2, copy);
}

TEST(Operators, MoveConstructor2) {
    AVL_Tree<int> avl{{1, 2, 3, 4, -1, -2, -3, -4, 0}};
    auto copy = avl;
    AVL_Tree<int> avl2 = std::move(avl);

    ASSERT_EQ(avl2, copy);
}

TEST(Operators, AssignmentOperator1) {
    AVL_Tree<int> avl{};
    AVL_Tree<int> avl2{};

    avl2 = avl;

    ASSERT_EQ(avl2, avl);
}

TEST(Operators, AssignmentOperator2) {
    AVL_Tree<int> avl{{1, -2, -1, 0, 14324, -5245}};
    AVL_Tree<int> avl2{};

    avl2 = avl;

    ASSERT_EQ(avl2, avl);
}

TEST(Operators, MoveAssignmentOperator1) {
    AVL_Tree<int> avl{};
    AVL_Tree<int> copy = avl;
    AVL_Tree<int> avl2{};

    avl2 = std::move(avl);

    ASSERT_EQ(avl2, copy);
}

TEST(Operators, MoveAssignmentOperator2) {
    AVL_Tree<int> avl{{1, -2, -1, 0, 14324, -5245}};
    AVL_Tree<int> copy = avl;
    AVL_Tree<int> avl2{};

    avl2 = std::move(avl);

    ASSERT_EQ(avl2, copy);
}
