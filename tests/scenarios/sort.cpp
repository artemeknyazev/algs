#define TEST
//#define DEBUG_PRINT

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <list>
#include <deque>

#include "algs/sort/selection.hpp"
#include "algs/sort/insertion.hpp"
#include "algs/sort/shell.hpp"
#include "../helpers/test_sort_on_container.hpp"

namespace {
    REGISTER_TESTS(Sort, SelectionSort, algs::sort::selection::sort)
    REGISTER_TESTS(Sort, SelectionSortSTL, algs::sort::selection::sort_stl)
    REGISTER_TESTS(Sort, InsertionSort, algs::sort::insertion::sort)
    REGISTER_TESTS(Sort, ShellSort, algs::sort::shell::sort)
}; // namespace

