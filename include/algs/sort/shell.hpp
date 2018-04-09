#include <iterator>

namespace algs::sort::shell {
    template<typename RandomAccessIterator>
    bool
    is_h_sorted(RandomAccessIterator begin,
                RandomAccessIterator end,
                typename std::iterator_traits<RandomAccessIterator>::difference_type h)
    {
        for (auto prev = begin, curr = std::next(begin, h);
             curr < end;
             prev = curr, curr = std::next(curr, h))
            if (*curr < *prev)
                return false;

        return true;
    }

    template<typename RandomAccessIterator>
    void
    sort(RandomAccessIterator begin,
         RandomAccessIterator end)
    {
        using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;

        difference_type hmax = std::distance(begin, end) / 3;
        difference_type h = 1;
        while (h < hmax)
            h = 3 * h + 1;

        for (; h > 0; h /= 3) {
            for (auto it = std::next(begin, h); it < end; ++it) {
                for (auto curr = it, prev = std::prev(it, h);
                     prev >= begin && *curr < *prev;
                     curr = prev, prev = std::prev(prev, h)) {
                    std::iter_swap(curr, prev);
                }
            }
#ifdef TEST
            ASSERT_TRUE(is_h_sorted(begin, end, h));
#endif // TEST
        }
    }
} // namespace algs::sort::shell

