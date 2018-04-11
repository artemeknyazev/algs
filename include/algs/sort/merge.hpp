namespace algs::sort::merge {
    /**
     * Merges two collections into a sorted output one.
     * NOTE: Requires input collections to be pre-sorted
     **/
    template<
        typename InputIterator1,
        typename InputIterator2,
        typename OutputIterator
    >
    OutputIterator
    merge(
        InputIterator1 left_begin,
        InputIterator1 left_end,
        InputIterator2 right_begin,
        InputIterator2 right_end,
        OutputIterator out_begin
    ) {
        assert(std::is_sorted(left_begin, left_end));
        assert(std::is_sorted(right_begin, right_end));

        auto it_left = left_begin;
        auto it_right = right_begin;
        auto it_out = out_begin;
        // NOTE: 3 checks always while one part is not exhausted
        while (it_left != left_end && it_right != right_end) {
            if (*it_left < *it_right) {
                *it_out = *it_left;
                ++it_left;
            } else {
                *it_out = *it_right;
                ++it_right;
            }
            ++it_out;
        }
        // NOTE: 1 out-of-loop check for the exhausted part, then
        //       1 in-loop for elements of the non-exhausted one
        // it_out = std::copy(it_left, left_end, it_out);
        while (it_left != left_end) {
            *it_out = *it_left;
            ++it_left;
            ++it_out;
        }
        // it_out = std::copy(it_right, right_end, it_out);
        while (it_right != right_end) {
            *it_out = *it_right;
            ++it_right;
            ++it_out;
        }
        return it_out;
    }

    /**
     * Merges left and right parts of an input
     * collection into a sorted output one.
     * NOTE: Requires both parts to be pre-sorted
     **/
    template<
        typename InputIterator,
        typename OutputIterator
    >
    OutputIterator
    merge(
        InputIterator left,
        InputIterator mid,
        InputIterator right,
        OutputIterator out
    ) {
        return algs::sort::merge::merge(left, mid, mid, right, out);
    }

    /**
     * Merges left and right parts of an input collection
     * through temporary into the original one.
     * NOTE: Requires tmp collection to be of a size
     * at least equal to std::distance(left, right).
     * NOTE: Requires both parts to be pre-sorted
     **/
    template<
        typename ForwardIterator,
        typename ForwardIteratorTmp
    >
    void
    merge_tmp(
        ForwardIterator begin,
        ForwardIterator mid,
        ForwardIterator end,
        ForwardIteratorTmp tmp
    ) {
        assert(std::is_sorted(begin, mid));
        assert(std::is_sorted(mid, end));

        auto tmp_begin = tmp;
        auto tmp_mid = std::copy(begin, mid, tmp_begin);
        auto tmp_end = std::copy(mid, end, tmp_mid);
        auto tmp_left = tmp_begin, tmp_right = tmp_mid;
        // NOTE: Sedgewicks' code ported from Java example
        // NOTE: 4 checks if both not exhausted, 3 checks if
        //       right exhausted, 2 checks if left
        // TODO: replace with merge(tmp_begin, tmp_mid, tmp_right, begin)
        //       and bench
        for (auto it = begin; it != end; ++it) {
            if (tmp_left == tmp_mid) {
                *it = *tmp_right;
                ++tmp_right;
            } else if (tmp_right == tmp_end) {
                *it = *tmp_left;
                ++tmp_left;
            } else if (*tmp_left < *tmp_right) {
                *it = *tmp_left;
                ++tmp_left;
            } else {
                *it = *tmp_right;
                ++tmp_right;
            }
        }
    }

    /**
     * Merges two collections in-place. Left collection will contain elements
     * less than or equal to the first element of the right collection. Both
     * collections will stay sorted.
     * NOTE: Requires both collections to be pre-sorted
     **/
    template<
        typename ForwardIterator1,
        typename ForwardIterator2
    >
    void
    inplace_merge(
        ForwardIterator1 left_begin,
        ForwardIterator1 left_end,
        ForwardIterator2 right_begin,
        ForwardIterator2 right_end
    ) {
        assert(std::is_sorted(left_begin, left_end));
        assert(std::is_sorted(right_begin, right_end));

        // scan left collection left-to-right
        for (auto it_left = left_begin; it_left != left_end; ++it_left) {
            // if found element gt than the first one of the second collection
            if (*right_begin < *it_left) {
                // swap them and bubble-up in the right collection
                // until right one is sorted again
                std::iter_swap(it_left, right_begin);
                for (auto curr_right = right_begin, next_right = std::next(curr_right);
                    next_right != right_end && *next_right < *curr_right;
                    curr_right = next_right, next_right = std::next(next_right))
                    std::iter_swap(curr_right, next_right);
            }
        }
    }

    /**
     * Merges two parts of a collection. Left part will contain elements
     * less than or equal to the first element of the right part.
     * NOTE: Requires both parts to be pre-sorted
     **/
    template<
        typename ForwardIterator
    >
    void
    inplace_merge(
        ForwardIterator begin,
        ForwardIterator mid,
        ForwardIterator end
    ) {
        algs::sort::merge::inplace_merge(begin, mid, mid, end);
    }

    /**
     * Implementation of a recursive merge sort. DO NOT USE DIRECTLY
     **/
    template<
        typename ForwardIterator,
        typename ForwardIteratorTmp
    >
    void
    sort_recursive_impl(
        ForwardIterator begin,
        ForwardIterator end,
        ForwardIteratorTmp tmp
    ) {
        auto size = std::distance(begin, end);
        if (size < 2)
            return;
        auto mid = std::next(begin, size / 2);
        sort_recursive_impl(begin, mid, tmp);
        sort_recursive_impl(mid, end, tmp);
        merge_tmp(begin, mid, end, tmp);
    }

    /**
     * Recursive merge sort.
     * NOTE: Uses temporary array of size std::distance(begin, end)
     **/
    template<
        typename ForwardIterator
    >
    void
    sort_recursive(
        ForwardIterator begin,
        ForwardIterator end
    ) {
        using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
        auto size = std::distance(begin, end);
        std::vector<value_type> tmp(size);
        sort_recursive_impl(begin, end, tmp.begin());
    }

    /**
     * In-place recursive merge sort
     **/
    template<
        typename ForwardIterator
    >
    void
    sort_recursive_inplace(
        ForwardIterator begin,
        ForwardIterator end
    ) {
        auto size = std::distance(begin, end);
        if (size < 2)
            return;
        auto mid = std::next(begin, size / 2);
        sort_recursive(begin, mid);
        sort_recursive(mid, end);
        algs::sort::merge::inplace_merge(begin, mid, end);
    }

    /**
     * Bottom-up merge sort.
     * NOTE: Uses temporary array of size std::distance(begin, end)
     * NOTE: Internal checks require RandomAccessIterator!
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sort_bottomup(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
        size_t max_size = std::distance(begin, end);
        std::vector<value_type> tmp(max_size);
        auto tmp_begin = tmp.begin();
        for (size_t size = 1; size < max_size; size *= 2) {
            auto it_stop = std::prev(end, size);
            for (auto it = begin; it < it_stop; std::advance(it, size+size))
                algs::sort::merge::merge_tmp(it, std::next(it, size), std::min(std::next(it, size+size), end), tmp_begin);
        }
    }

    /**
     * Bottom-up in-place merge sort.
     * NOTE: Internal checks require RandomAccessIterator!
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sort_bottomup_inplace(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        size_t max_size = std::distance(begin, end);
        for (size_t size = 1; size < max_size; size *= 2) {
            auto it_stop = std::prev(end, size);
            for (auto it = begin; it < it_stop; std::advance(it, size+size))
                algs::sort::merge::inplace_merge(it, std::next(it, size), std::min(std::next(it, size+size), end));
        }
    }
} // namespace algs::sort::merge

