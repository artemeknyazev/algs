#define TEST

#include <gtest/gtest.h>
#include "algs/sort/selection.hpp"
#include "algs/sort/insertion.hpp"
#include "algs/sort/shell.hpp"

namespace {
    using namespace algs::sort;

    TEST(Sort, SelectionSort) {
        std::vector<int> in = { 5, 4, 3, 2, 1 };
        std::vector<int> out = in;
        std::sort(out.begin(), out.end());

        selection::sort(in.begin(), in.end());
        ASSERT_TRUE(std::is_sorted(in.begin(), in.end()));
        ASSERT_EQ(in, out);
    }

    TEST(Sort, SelectionSortStl) {
        std::vector<int> in = { 5, 4, 3, 2, 1 };
        std::vector<int> out = in;
        std::sort(out.begin(), out.end());

        selection::sort_stl(in.begin(), in.end());
        ASSERT_TRUE(std::is_sorted(in.begin(), in.end()));
        ASSERT_EQ(in, out);
    }

    TEST(Sort, InsertionSort) {
        std::vector<int> in = { 5, 4, 3, 2, 1 };
        std::vector<int> out = in;
        std::sort(out.begin(), out.end());

        insertion::sort(in.begin(), in.end());
        ASSERT_TRUE(std::is_sorted(in.begin(), in.end()));
        ASSERT_EQ(in, out);
    }

    TEST(Sort, ShellSort) {
        std::vector<int> in = { 5, 4, 3, 2, 1 };
        std::vector<int> out = in;
        std::sort(out.begin(), out.end());

        shell::sort(in.begin(), in.end());
        ASSERT_TRUE(std::is_sorted(in.begin(), in.end()));
        ASSERT_EQ(in, out);
    }
}; // namespace

