#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <list>
#include <deque>

#include "algs/sort/selection.hpp"
#include "algs/sort/insertion.hpp"
#include "algs/sort/shell.hpp"
#include "algs/sort/merge.hpp"
#include "../helpers/test_sort_on_container.hpp"

namespace {
    REGISTER_TESTS(Sort, SelectionSort, algs::sort::selection::sort)
    REGISTER_TESTS(Sort, SelectionSortSTL, algs::sort::selection::sort_stl)
    REGISTER_TESTS(Sort, InsertionSort, algs::sort::insertion::sort)
    REGISTER_TESTS(Sort, ShellSort, algs::sort::shell::sort)
    REGISTER_TESTS(Sort, MergeSortRecursive, algs::sort::merge::sort_recursive)
    REGISTER_TESTS(Sort, MergeSortRecursiveInplace, algs::sort::merge::sort_recursive_inplace)
    REGISTER_TESTS(Sort, MergeSortBottomup, algs::sort::merge::sort_bottomup)
    REGISTER_TESTS(Sort, MergeSortBottomupInplace, algs::sort::merge::sort_bottomup_inplace)

    TEST(Sort, Merge) {
        using Container = std::vector<int>;
        using value_type = Container::value_type;
        std::random_device rd;
        std::mt19937 gen(rd());
        for (const auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                // prepare to-be merged collections
                std::uniform_int_distribution<> dis(0, size-1);
                int mid = dis(gen);
                Container left(mid);
                Container right(size-mid);
                fill_container(left.begin(), left.end(), value_type(0), value_type(size));
                std::sort(left.begin(), left.end());
                fill_container(right.begin(), right.end(), value_type(0), value_type(size));
                std::sort(right.begin(), right.end());

                // produce reference collection
                Container reference(size);
                std::merge(left.begin(), left.end(), right.begin(), right.end(), reference.begin());

                // produce test data
                Container result(size);
                algs::sort::merge::merge(left.begin(), left.end(), right.begin(), right.end(), result.begin());

                // check
                ASSERT_TRUE(std::equal(result.begin(), result.end(), reference.begin(), reference.end()));
            }
        }
    }

    TEST(Sort, MergeTmp) {
        using Container = std::vector<int>;
        using value_type = Container::value_type;
        std::random_device rd;
        std::mt19937 gen(rd());
        for (const auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                // prepare to-be merged collection
                std::uniform_int_distribution<> dis(0, size-1);
                Container cont(size);
                auto mid = std::next(cont.begin(), dis(gen));
                fill_container(cont.begin(), cont.end(), value_type(0), value_type(size));
                std::sort(cont.begin(), mid);
                std::sort(mid, cont.end());

                // produce reference collection using std::merge
                Container reference(size);
                std::merge(cont.begin(), mid, mid, cont.end(), reference.begin());

                // produce test data
                Container tmp(size);
                algs::sort::merge::merge_tmp(cont.begin(), mid, cont.end(), tmp.begin());

                // check
                ASSERT_TRUE(std::equal(cont.begin(), cont.end(), reference.begin(), reference.end()));
            }
        }
    }

    TEST(Sort, InplaceMerge) {
        using Container = std::vector<int>;
        using value_type = Container::value_type;
        std::random_device rd;
        std::mt19937 gen(rd());
        for (const auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                // prepare to-be-merged collection
                std::uniform_int_distribution<> dis(0, size-1);
                Container cont(size);
                auto mid = dis(gen);
                auto cont_mid = std::next(cont.begin(), mid);
                fill_container(cont.begin(), cont.end(), value_type(0), value_type(size));
                std::sort(cont.begin(), cont_mid);
                std::sort(cont_mid, cont.end());

                // produce reference collections
                auto reference = cont;
                auto reference_mid = std::next(reference.begin(), mid);
                std::inplace_merge(reference.begin(), reference_mid, reference.end());

                // produce test data
                algs::sort::merge::inplace_merge(cont.begin(), cont_mid, cont.end());

                // check
                ASSERT_TRUE(std::equal(cont.begin(), cont.end(), reference.begin(), reference.end()));
            }
        }
    }
}; // namespace

