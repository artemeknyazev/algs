#include <algorithm>
#include <iterator>

namespace algs::sort::selection {
    template<typename ForwardIterator>
    void
    sort(ForwardIterator begin,
         ForwardIterator end)
    {
        for (; begin != end; ++begin) {
            auto itmin = begin;
            for (auto itt = std::next(itmin); itt != end; ++itt)
                if (*itt < *itmin)
                    itmin = itt;
            std::iter_swap(begin, itmin);
        }
    }

    template<typename ForwardIterator>
    void
    sort_stl(ForwardIterator begin,
             ForwardIterator end)
    {
        for (; begin != end; ++begin)
            std::iter_swap(begin, std::min_element(begin, end));
    }
}; // namespace algs::sort::selection

