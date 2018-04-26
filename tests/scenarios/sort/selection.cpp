#include "common.hpp"
#include "algs/sort/selection.hpp"

namespace {
    REGISTER_TESTS(Sort, Selection_Sort, algs::sort::selection::sort)
    REGISTER_TESTS(Sort, Selection_SortSTL, algs::sort::selection::sort_stl)
}

