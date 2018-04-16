#include "algs/tree/bst_recursive.hpp"

namespace {
    using bst = algs::tree::bst::bst_recursive<int, int>;

    TEST(Tree, BST_Recursive_Empty) {
        bst tree;
        ASSERT_EQ(tree.size(), 0);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 0));
        ASSERT_EQ(tree.find_min(), std::make_pair(0, 0));
        ASSERT_EQ(tree.find_max(), std::make_pair(0, 0));
    }

    TEST(Tree, BST_Recursive_Simple) {
        bst tree;
        tree.insert(1, 2);
        ASSERT_EQ(tree.size(), 1);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_FALSE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
        ASSERT_EQ(tree.find_min(), std::make_pair(1, 2));
        ASSERT_EQ(tree.find_max(), std::make_pair(1, 2));
        tree.insert(2, 4);
        ASSERT_EQ(tree.size(), 2);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_FALSE(tree.contains(3));
        ASSERT_EQ(tree.find_min(), std::make_pair(1, 2));
        ASSERT_EQ(tree.find_max(), std::make_pair(2, 4));
        tree.insert(3, 8);
        ASSERT_EQ(tree.size(), 3);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
        ASSERT_EQ(tree.find_min(), std::make_pair(1, 2));
        ASSERT_EQ(tree.find_max(), std::make_pair(3, 8));
        tree.insert(0, 1);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 1));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
        ASSERT_EQ(tree.find_min(), std::make_pair(0, 1));
        ASSERT_EQ(tree.find_max(), std::make_pair(3, 8));
    }
}

