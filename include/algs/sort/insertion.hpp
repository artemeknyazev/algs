#include <iterator>
#include <iostream>

namespace algs::sort::insertion {
    template<typename BidirectionalIterator>
    void
    sort(BidirectionalIterator begin,
         BidirectionalIterator end)
    {
        for (auto it = std::next(begin); it != end; ++it) {
            auto curr = it, prev = std::prev(it);
            while (curr != begin && *curr < *prev) {
                std::iter_swap(curr, prev);
                curr = prev--;
            }
        }
    }
} // namespace algs::sort::insertion

