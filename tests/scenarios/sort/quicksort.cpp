#include "algs/sort/quicksort.hpp"
#include "algs/sort/shuffle.hpp"

namespace {
    REGISTER_TESTS(Sort, Quicksort_Sort, algs::sort::quicksort::sort)

    TEST(Sort, Quicksort_IsPartitioned) {
        using algs::sort::quicksort::is_partitioned;
        { // check empty
            std::vector<int> original = {};
            auto pivot = original.begin();
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
        }
        { // check one element
            std::vector<int> original = {1};
            auto pivot = original.begin();
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
        }
        { // check sorted
            std::vector<int> original = {1,2,3,4,5,6,7,8,9,10};
            auto pivot = original.begin();
            while (pivot != original.end()) {
                ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
                ++pivot;
            }
        }
        { // check partitioned
            std::vector<int> original = {0,5,4,2,6,9,8,10,12,7};
            auto pivot = std::next(original.begin(), 4);
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
        }
        { // check 3-way partitioned
            std::vector<int> original = {0,5,4,2,6,6,6,10,12,7};
            auto pivot = std::next(original.begin(), 4);
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
            pivot = std::next(original.begin(), 5);
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
            pivot = std::next(original.begin(), 6);
            ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
        }
    }

    TEST(Sort, Quicksort_Partition) {
        using algs::sort::quicksort::partition;
        using algs::sort::quicksort::is_partitioned;
        using Container = std::vector<int>;
        using value_type = typename Container::value_type;
        for (auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                Container original(size);
                fill_container(original.begin(), original.end(), value_type(0), value_type(size));
                auto pivot = partition(original.begin(), original.end());
                ASSERT_TRUE(is_partitioned(original.begin(), pivot, original.end()));
            }
        }
    }

    TEST(Sort, Quicksort_Select) {
        using algs::sort::shuffle;
        using algs::sort::quicksort::select;
        using Container = std::vector<int>;
        using value_type = typename Container::value_type;
        std::random_device rd;
        std::mt19937 gen(rd());
        for (auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                std::uniform_int_distribution<> dis(0, size-1);
                Container original(size);
                size_t k = dis(gen);
                std::generate(original.begin(), original.end(), [n = 0] () mutable { return value_type(n++); });
                shuffle(original.begin(), original.end());
                auto it = select(original.begin(), original.end(), k);
                ASSERT_TRUE(*it == value_type(k));
            }
        }
    }

    TEST(Sort, Quicksort_Partition_DualPivot) {
        using algs::sort::quicksort::partition_dual_pivot;
        using algs::sort::quicksort::is_partitioned_dual_pivot;
        using Container = std::vector<int>;
        using value_type = typename Container::value_type;
        for (auto size : TEST_CONTAINER_SIZES) {
            for (int i = 0; i < std::max(1., std::log(size)); ++i) {
                Container v(size);
                fill_container(v.begin(), v.end(), value_type(0), value_type(size));
                auto part = partition_dual_pivot(v.begin(), v.end());
                ASSERT_TRUE(is_partitioned_dual_pivot(v.begin(), part.first, part.second, v.end()));
            }
        }
    }
}

