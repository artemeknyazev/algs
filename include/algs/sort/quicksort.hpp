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
        if (begin == end)
            return true;
        auto it = begin;
        while (it != pivot)
            if (*it <= *pivot)
                ++it;
            else {
                std::cout << "aaa " << *it << std::endl;
                return false;
            }
        it = std::prev(end);
        while (it != pivot)
            if (*pivot <= *it)
                --it;
            else {
                std::cout << "bbb " << *it << std::endl;
                return false;
            }
        return true;
    }

    /**
     * Partition a collection around a pivot element.
     * NOTE: Uses first element as a pivot
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

