#include "algs/sort/heap.hpp"

namespace {
    TEST(Sort, Heap_IsMaxHeap) {
        std::vector<int> emptyHeap = {};
        ASSERT_TRUE(algs::sort::heap::is_max_heap(emptyHeap.begin(), emptyHeap.end()));
        std::vector<int> oneElementHeap = { 1 };
        ASSERT_TRUE(algs::sort::heap::is_max_heap(oneElementHeap.begin(), oneElementHeap.end()));
        std::vector<int> twoElementHeap = { 2, 1 };
        ASSERT_TRUE(algs::sort::heap::is_max_heap(twoElementHeap.begin(), twoElementHeap.end()));
        std::vector<int> notFullHeap = { 5, 4, 3, 2, 1 };
        ASSERT_TRUE(algs::sort::heap::is_max_heap(notFullHeap.begin(), notFullHeap.end()));
        std::vector<int> notAHeap1 = { 6, 4, 3, 4, 5 };
        ASSERT_FALSE(algs::sort::heap::is_max_heap(notAHeap1.begin(), notAHeap1.end()));
        std::vector<int> notAHeap2 = { 6, 4, 3, 4, 4, 5 };
        ASSERT_FALSE(algs::sort::heap::is_max_heap(notAHeap2.begin(), notAHeap2.end()));
    }

    TEST(Sort, Heap_SiftUp) {
        {
            std::vector<int> original = {};
            std::vector<int> reference = {};
            algs::sort::heap::sift_up(original.begin(), original.begin());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1};
            std::vector<int> reference = {1};
            algs::sort::heap::sift_up(original.begin(), original.begin());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1,2};
            std::vector<int> reference = {2,1};
            algs::sort::heap::sift_up(original.begin(), std::next(original.begin()));
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {2,1};
            std::vector<int> reference = {2,1};
            algs::sort::heap::sift_up(original.begin(), std::next(original.begin()));
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {8,5,6,4,4,9};
            std::vector<int> reference = {9,5,8,4,4,6};
            algs::sort::heap::sift_up(original.begin(), std::next(original.begin(), 5));
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
    }

    TEST(Sort, Heap_SiftDown) {
        {
            std::vector<int> original = {};
            std::vector<int> reference = {};
            algs::sort::heap::sift_down(original.begin(), original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1};
            std::vector<int> reference = {1};
            algs::sort::heap::sift_down(original.begin(), original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {2,1};
            std::vector<int> reference = {2,1};
            algs::sort::heap::sift_down(original.begin(), original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1,2};
            std::vector<int> reference = {2,1};
            algs::sort::heap::sift_down(original.begin(), original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {9, 0,6, 3,2,5,4, 1,2,2,1,4,3,3,2};
            std::vector<int> reference = {9, 3,6, 2,2,5,4, 1,0,2,1,4,3,3,2};
            algs::sort::heap::sift_down(original.begin(), std::next(original.begin()), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
    }

    TEST(Sort, Heap_MakeHeap) {
        {
            std::vector<int> original = {};
            std::vector<int> reference = {};
            algs::sort::heap::make_heap(original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1};
            std::vector<int> reference = {1};
            algs::sort::heap::make_heap(original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1,2};
            std::vector<int> reference = {2,1};
            algs::sort::heap::make_heap(original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
        {
            std::vector<int> original = {1,2,3,4,5};
            std::vector<int> reference = {5,4,3,1,2};
            algs::sort::heap::make_heap(original.begin(), original.end());
            ASSERT_TRUE(std::equal(original.begin(), original.end(), reference.begin(), reference.end()));
        }
    }

    REGISTER_TESTS(Sort, Heap_Sort, algs::sort::heap::sort)
}

