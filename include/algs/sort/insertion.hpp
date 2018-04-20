#include <iterator>
#include <iostream>

namespace algs::sort::insertion {
    template<
        typename BidirectionalIterator
    >
    void
    sort(
        BidirectionalIterator begin,
        BidirectionalIterator end
    ) {
        for (auto it = std::next(begin); it != end; ++it) {
            auto curr = it, prev = std::prev(it);
            while (curr != begin && *curr < *prev) {
                std::iter_swap(curr, prev);
                curr = prev--;
            }
        }
    }

    /**
     * Enhanced version of an insertion sort from Sedgewick's "Algorithms in C++"
     **/
    template<
        typename BidirectionalIterator
    >
    void
    sort_enhanced(
        BidirectionalIterator begin,
        BidirectionalIterator end
    ) {
        // guarantees that the first element contains a collection's minimum
        for (auto curr = std::prev(end), prev = std::prev(curr);
             curr != begin;
             curr = prev, prev = std::prev(curr))
            if (*curr < *prev)
                std::iter_swap(curr, prev);
        // extract - shift-until - replace loop for all elems
        // stops because the first element always contains a minimum
        for (auto it = std::next(begin, 2); it != end; ++it) {
            auto value = std::move(*it);
            auto curr = it;
            for (auto prev = std::prev(curr);
                 value < *prev;
                 curr = prev, prev = std::prev(curr))
                *curr = std::move(*prev);
            *curr = std::move(value);
        }
    }
} // namespace algs::sort::insertion

