#include <iterator>

namespace algs::sort::heap {
    /**
     * Check if an input collection is a max heap
     **/
    template<
        typename RandomAccessIterator
    >
    bool
    is_max_heap(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        auto max_parent = std::next(begin, std::distance(begin, end) / 2);
        for (auto parent = begin; parent != max_parent; ++parent) {
            // left child always present because of max_parent
            auto child = std::next(begin, 2 * std::distance(begin, parent) + 1);
            if (*parent < *child)
                return false;
            // last parent can have either one or to children
            std::advance(child, 1);
            if (child == end)
                return true;
            if (*parent < *child)
                return false;
        }
        return true;
    }

    /**
     * Sift up `current` element until `begin` element in max heap
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sift_up(
        RandomAccessIterator begin,
        RandomAccessIterator current
    ) {
        assert(begin <= current);

        while (current != begin) {
            auto parent = std::next(begin, (std::distance(begin, current) - 1) / 2);
            if (*current <= *parent)
                break;
            std::iter_swap(current, parent);
            current = parent;
        }
    }

    /**
     * Sift down `current` element in `begin`-`end` max heap
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sift_down(
        RandomAccessIterator begin,
        RandomAccessIterator current,
        RandomAccessIterator end
    ) {
        assert(begin <= current);
        assert(current <= end);

        while (current < end) {
            auto swap = current;
            auto child = std::next(begin, std::distance(begin, current) * 2 + 1);
            if (child < end && *swap < *child) // left child
                swap = child;
            std::advance(child, 1);
            if (child < end && *swap < *child) // right child
                swap = child;
            if (swap == current)
                break;
            std::iter_swap(swap, current);
            current = swap;
        }
    }

    /**
     * Make heap from collection
     **/
    template<
        typename RandomAccessIterator
    >
    void
    make_heap(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        auto current = std::next(begin, std::distance(begin, end) / 2 - 1);
        while (std::distance(begin, current) >= 0) {
            sift_down(begin, current, end);
            --current;
        }
    }

    /**
     * Heap sort
     **/
    template<
        typename RandomAccessIterator
    >
    void
    sort(
        RandomAccessIterator begin,
        RandomAccessIterator end
    ) {
        algs::sort::heap::make_heap(begin, end);
        auto last = std::prev(end);
        while (std::distance(begin, last) >= 0) {
            std::iter_swap(begin, last);
            end = last;
            --last;
            sift_down(begin, begin, end);
        }
    }
} // namespace algs::sort::heap

