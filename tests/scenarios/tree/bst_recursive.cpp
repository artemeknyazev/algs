#include <random>
#include <algorithm>
#include "algs/tree/bst_recursive.hpp"

namespace {
    using bst = algs::tree::bst::bst_recursive<int, int>;

    TEST(Tree, BST_Recursive_Empty) {
        bst tree;
        ASSERT_EQ(tree.size(), 0);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 0));
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
        tree.insert(2, 4);
        ASSERT_EQ(tree.size(), 2);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_FALSE(tree.contains(3));
        tree.insert(3, 8);
        ASSERT_EQ(tree.size(), 3);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
        tree.insert(0, 1);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_EQ(tree.find(0), std::make_pair(0, 1));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_EQ(tree.find(1), std::make_pair(1, 2));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_EQ(tree.find(2), std::make_pair(2, 4));
        ASSERT_TRUE(tree.contains(3));
        ASSERT_EQ(tree.find(3), std::make_pair(3, 8));
    }

    TEST(Tree, BST_Recursive_RemoveNonexistent) {
        bst tree;
        tree.insert(2, 4);
        tree.insert(0, 1);
        tree.insert(1, 2);
        tree.insert(3, 8);
        tree.remove(5);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_TRUE(tree.contains(3));
        tree.remove(-1);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_TRUE(tree.contains(3));
    }

    TEST(Tree, BST_Recursive_RemoveLeaf) {
        bst tree;
        tree.insert(2, 4);
        tree.insert(0, 1);
        tree.insert(1, 2);
        tree.insert(3, 8);
        tree.remove(3);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_TRUE(tree.contains(1));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
        tree.remove(1);
        ASSERT_TRUE(tree.contains(0));
        ASSERT_FALSE(tree.contains(1));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
        tree.remove(0);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_FALSE(tree.contains(1));
        ASSERT_TRUE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
        tree.remove(2);
        ASSERT_FALSE(tree.contains(0));
        ASSERT_FALSE(tree.contains(1));
        ASSERT_FALSE(tree.contains(2));
        ASSERT_FALSE(tree.contains(3));
    }

    TEST(Tree, BST_Recursive_Debug_InOrder) {
        for (size_t size = 16; size <= 1 << 12; size <<= 1) {
            std::vector<int> base(size);
            std::generate(base.begin(), base.end(), [n = 0] () mutable { return n++; });
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(base.begin(), base.end(), g);

            std::vector<bst::value_type> reference(size);
            std::transform(base.begin(), base.end(), reference.begin(),
                [] (int a) { return std::make_pair(a, a+1); });
            std::sort(reference.begin(), reference.end(),
                [] (bst::value_type a, bst::value_type b) { return a.first < b.first; });

            bst tree;
            for (const auto& it : base)
                tree.insert(it, it+1);

            std::vector<bst::value_type> ordered;
            tree.in_order(ordered);

            bool flag = std::equal(ordered.begin(), ordered.end(), reference.begin(), reference.end());
            ASSERT_TRUE(flag);
        }
    }

    TEST(Tree, BST_Recursive_Debug_InOrderReverse) {
        for (size_t size = 16; size <= 1 << 12; size <<= 1) {
            std::vector<int> base(size);
            std::generate(base.begin(), base.end(), [n = 0] () mutable { return n++; });
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(base.begin(), base.end(), g);

            std::vector<bst::value_type> reference(size);
            std::transform(base.begin(), base.end(), reference.begin(),
                [] (int a) { return std::make_pair(a, a+1); });
            std::sort(reference.begin(), reference.end(),
                [] (bst::value_type a, bst::value_type b) { return a.first > b.first; });

            bst tree;
            for (const auto& it : base)
                tree.insert(it, it+1);

            std::vector<bst::value_type> ordered;
            tree.in_order_reverse(ordered);

            bool flag = std::equal(ordered.begin(), ordered.end(), reference.begin(), reference.end());
            ASSERT_TRUE(flag);
        }
    }

    TEST(Tree, BST_Recursive_Remove_Randomized) {
        for (size_t size = 16; size <= 1 << 12; size <<= 1) {
            std::vector<int> base(size);
            std::generate(base.begin(), base.end(), [n = 0] () mutable { return n++; });
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(base.begin(), base.end(), g);

            std::vector<bst::value_type> reference(size);
            std::transform(base.begin(), base.end(), reference.begin(),
                [] (int a) { return std::make_pair(a, a+1); });
            std::sort(reference.begin(), reference.end(),
                [] (bst::value_type a, bst::value_type b) { return a.first < b.first; });

            bst tree;
            for (const auto& it : base)
                tree.insert(it, it+1);

            for (size_t sz = tree.size(); sz; --sz) {
                std::uniform_int_distribution<> d(0, sz-1);
                auto pos = d(g);
                tree.remove(base[pos]);
                reference.erase(std::find_if(reference.begin(), reference.end(),
                    [&](const bst::value_type& val) { return val.first == base[pos]; }));
                base.erase(base.begin() + pos);

                std::vector<bst::value_type> ordered;
                tree.in_order(ordered);

                bool flag = std::equal(ordered.begin(), ordered.end(), reference.begin(), reference.end());
                ASSERT_TRUE(flag);
            }
        }
    }
}

