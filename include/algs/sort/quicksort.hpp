#include <utility>

// TODO: add 3-way partitioning;

namespace algs::sort::quicksort {
    /**
     * Check if a collection is partitioned around a `pivot` element
     **/
    template<
        typename RandomAccessIterator
    >
    bool
    is_partitioned(
        RandomAccessIterator begin,
        RandomAccessIterator pivot,
        RandomAccessIterator end
    ) {
        assert(begin <= pivot);
        assert(pivot <= end);

        if (begin == end)
            return true;
        auto it = begin;
        while (it != pivot)
            if (*it <= *pivot)
                ++it;
            else {
                //std::cout << "aaa " << *it << std::endl;
                return false;
            }
        it = std::prev(end);
        while (it != pivot)
            if (*pivot <= *it)
                --it;
            else {
                //std::cout << "bbb " << *it << std::endl;
                return false;
            }
        return true;
    }

    /**
     * Check if a collection is partitioned around dual pivots
     **/
    template<
        typename RandomAccessIterator
    >
    bool
    is_partitioned_dual_pivot(
        RandomAccessIterator begin,
        RandomAccessIterator left,
        RandomAccessIterator right,
        RandomAccessIterator end
    ) {
        assert(begin <= left);
        assert(left <= right);
        assert(right <= end);

        if (begin == end)
            return true;

        auto it = begin;
        while (it != left)
            if (*it < *left)
                ++it;
            else {
                //std::cerr << "aaa " << *it << " " << *left << std::endl;
                return false;
            }
        it = std::next(left);
        while (it != right)
            if (*left <= *it && *it <= *right)
                ++it;
            else {
                //std::cerr << "bbb " << *it << " " << *left << " " << *right << std::endl;
                return false;
            }
        it = std::next(right);
        while (it != end)
            if (*left < *it)
                ++it;
            else {
                //std::cerr << "ccc " << *it << " " << *right << std::endl;
                return false;
            }
        return true;
    }

    /**
     * Partition a collection around a pivot element.
     * NOTE: Uses first element as a pivot
     * NOTE: New element order: (<=p) p (>p)
     **/
    template<
        typename RandomAccessIterator
    >
    RandomAccessIterator
    partition(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        auto less = std::next(begin);
        auto more = std::prev(end);
        while (true) {
            while (less != end && *less <= *begin)
                ++less;
            while (more != begin && *begin < *more)
                --more;
            if (less >= more)
                break;
            std::iter_swap(less, more);
        }
        std::iter_swap(begin, more);
        return more;
    }

    /**
     * Partition a collection around two pivots
     * NOTE: Uses first and last elements as pivots
     * NOTE: New element order: (<lp) lp (lp <= & <= rp) rp (rp<)
     **/
    template<
        typename RandomAccessIterator
    >
    std::pair<RandomAccessIterator, RandomAccessIterator>
    partition_dual_pivot(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        if (end <= begin)
            return std::make_pair(begin, end);
        auto p = begin; // left pivot
        auto q = std::prev(end); // right pivot
        if (*q < *p) // left pivot should be less than the right one
            std::iter_swap(p, q);
        // (p,l) contains elems less than the first pivot
        // [l,k) contains elems between pivots
        // [k,g] contains unpartitioned part of the collection
        // (g,q) contains elems greater than the second pivot
        auto l = std::next(begin);
        auto g = std::prev(end);
        for (auto k = l; k <= g; ++k) {
            if (*k < *p) { // skip elems less than the left pivot
                std::iter_swap(k, l);
                ++l;
                //std::cout << "a:  "; for (auto it = begin; it != end; ++it) std::cout << *it << ' '; std::cout << std::endl;
            } else if (*q <= *k) { // found element in the left part gte than the right pivot
                while (k < g && *q < *g) // skip elems less than the right pivot
                    --g;
                std::iter_swap(k, g); // swap left greater elem with right less elem
                //std::cout << "b1: "; for (auto it = begin; it != end; ++it) std::cout << *it << ' '; std::cout << std::endl;
                --g;
                if (*k < *p) { // again skip elems less than the left pivot
                    std::iter_swap(k, l);
                    ++l;
                }
                //std::cout << "b2: "; for (auto it = begin; it != end; ++it) std::cout << *it << ' '; std::cout << std::endl;
            }
        }
        --l;
        ++g;
        std::iter_swap(p, l);
        std::iter_swap(q, g);
        return std::make_pair(l, g);
    }

    /**
     * Find k-th minimal element in a collection
     * NOTE: no implicit shuffle for future benching purposes
     **/
    template<
        typename RandomAccessIterator
    >
    RandomAccessIterator
    select(
        RandomAccessIterator begin,
        RandomAccessIterator end,
        size_t k
    ) {
        if (end < std::next(begin, k))
            return end;

        auto left = begin, right = end, it = std::next(begin, k);
        while (left != right) {
            auto pivot = algs::sort::quicksort::partition(left, right);
            if (pivot < it)
                left = std::next(pivot);
            else if (it < pivot)
                right = pivot;
            else
                return pivot;
        }

        return end;
    }

    /**
     * Standart quicksort implementation.
     * NOTE:: no implicit shuffle for future benching purposes
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sort(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        if (end <= begin)
            return;
        auto pivot = algs::sort::quicksort::partition(begin, end);
        algs::sort::quicksort::sort(begin, pivot);
        algs::sort::quicksort::sort(std::next(pivot), end);
    }
}

