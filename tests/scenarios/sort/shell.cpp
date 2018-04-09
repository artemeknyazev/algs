#define TEST

#include "gtest/gtest.h"
#include "algs/sort/shell.hpp"

namespace {
    using namespace algs::sort;

    TEST(Sort, ShellSort) {
        std::vector<int> in = { 5, 4, 3, 2, 1 };
        std::vector<int> out = in;
        std::sort(out.begin(), out.end());

        shell::sort(in.begin(), in.end());
        ASSERT_TRUE(std::is_sorted(in.begin(), in.end()));
        ASSERT_EQ(in, out);
    }

}; // namespace

