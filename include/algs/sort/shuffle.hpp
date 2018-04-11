#include <random>

namespace algs::sort {
    template<typename RandomAccessIterator>
    void shuffle(RandomAccessIterator begin, RandomAccessIterator end) {
        std::random_device rd;
        std::mt19937 gen(rd());

        for (auto it = begin; it != end; ++it) {
            std::uniform_int_distribution<> dis(0, it - begin);
            std::iter_swap(it, begin + dis(gen));
        }
    }
} // namespace algs::sort

