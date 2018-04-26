#include "common.hpp"
#include "algs/tree/avl.hpp"

namespace {
    using avl = algs::tree::avl::avl_test<int, int>;

    TEST(Tree, AVL_Empty) {
        avl tree;
        ASSERT_EQ(tree.size(), 0);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 0));
        ASSERT_TRUE(tree.check_links_valid());
        ASSERT_TRUE(tree.check_is_bst());
        ASSERT_TRUE(tree.check_is_avl());
    }

    TEST(Tree, AVL_Recursive_Simple) {
        avl tree;
        tree.insert(1, 2);
        ASSERT_EQ(tree.size(), 1);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_FALSE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
        ASSERT_TRUE(tree.check_links_valid());
        ASSERT_TRUE(tree.check_is_bst());
        ASSERT_TRUE(tree.check_is_avl());
        tree.insert(2, 4);
        ASSERT_EQ(tree.size(), 2);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_FALSE(tree.contains(3));
        ASSERT_TRUE(tree.check_links_valid());
        ASSERT_TRUE(tree.check_is_bst());
        ASSERT_TRUE(tree.check_is_avl());
        tree.insert(3, 8);
        ASSERT_EQ(tree.size(), 3);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
        ASSERT_TRUE(tree.check_links_valid());
        ASSERT_TRUE(tree.check_is_bst());
        ASSERT_TRUE(tree.check_is_avl());
        tree.insert(0, 1);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 1));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
        ASSERT_TRUE(tree.check_links_valid());
        ASSERT_TRUE(tree.check_is_avl());
    }

    TEST(Tree, AVL_Recursive_Insert_Randomized) {
        for (size_t size = 16; size <= 1 << 12; size <<= 1) {
            std::vector<int> base(size);
            std::generate(base.begin(), base.end(), [n = 0] () mutable { return n++; });
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(base.begin(), base.end(), g);

            avl tree;
            for (size_t i = 0; i < base.size(); ++i) {
                tree.insert(base[i], base[i]+1);
                for (size_t j = 0; j <= i; ++j)
                    ASSERT_EQ(tree.find(base[j]), std::make_pair(base[j], base[j]+1));

                ASSERT_TRUE(tree.check_links_valid());
                ASSERT_TRUE(tree.check_is_bst());
                ASSERT_TRUE(tree.check_is_avl());
            }
        }
    }
}

