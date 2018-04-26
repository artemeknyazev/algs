#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <random>

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v) {
    for (const auto& it : v)
        out << it << ' ';
    return out;
}

const size_t MAX_CONTAINER_SIZE = 1<<12;
const std::vector<size_t> TEST_CONTAINER_SIZES = {
    4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
    1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 1<<11, 1<<12
};

template<typename Container>
using ContainerSortFn =
    void (*)(typename Container::iterator,
             typename Container::iterator);

template<typename Iter>
using IterSortFn =
    void (*)(Iter, Iter);

// todo: randomized user-defined types to test multi-key comparison functions?
template<typename ForwardIterator,
         typename T = typename std::iterator_traits<ForwardIterator>::value_type>
void
fill_container(ForwardIterator begin,
               ForwardIterator end,
               T lo = std::numeric_limits<T>::lowest(),
               T hi = std::numeric_limits<T>::max())
{
    std::function<T()> fn;
    std::random_device rd;
    std::mt19937 gen(rd());
    if constexpr(std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(lo, hi);
        fn = [&] { return dis(gen); };
    } else if constexpr(std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dis(lo, hi);
        fn = [&] { return dis(gen); };
    } else
        throw new std::exception();
    std::generate(begin, end, fn);
}

template<typename Iter1,
         typename Iter2>
void test_sort_on_container(
    Iter1 cont_begin,
    Iter2 rcont_begin,
    IterSortFn<Iter1> sortFn,
    size_t size,
    typename std::iterator_traits<Iter1>::value_type lo 
        = std::numeric_limits<
            typename std::iterator_traits<Iter1>::value_type>::lowest(),
    typename std::iterator_traits<Iter1>::value_type hi
        = std::numeric_limits<
            typename std::iterator_traits<Iter1>::value_type>::max()
) {
    auto cont_end = std::next(cont_begin, size);
    fill_container(cont_begin, cont_end, lo, hi);
    auto rcont_end = std::copy(cont_begin, cont_end, rcont_begin);

#ifdef DEBUG_PRINT
    std::cout << "Original:  ";
    for (auto it = cont_begin; it != cont_end; ++it)
        std::cout << *it << ' ';
    std::cout << std::endl;
#endif

    std::sort(rcont_begin, rcont_end);
    sortFn(cont_begin, cont_end);

#ifdef DEBUG_PRINT
    std::cout << "Sorted:    ";
    for (auto it = cont_begin; it != cont_end; ++it)
        std::cout << *it << ' ';
    std::cout << std::endl;
#endif

#ifdef DEBUG_PRINT
    std::cout << "Reference: ";
    for (auto it = rcont_begin; it != rcont_end; ++it)
        std::cout << *it << ' ';
    std::cout << std::endl;
#endif

    ASSERT_TRUE(std::equal(cont_begin, cont_end, rcont_begin));
}

// todo: fix to be able to test on native arrays and std::array
template<typename Container>
void test_sort(ContainerSortFn<Container> sortFn)
{
    Container cont(MAX_CONTAINER_SIZE);
    Container rcont(MAX_CONTAINER_SIZE);

    for (const auto& sz : TEST_CONTAINER_SIZES) {
        assert(sz <= MAX_CONTAINER_SIZE);
        test_sort_on_container(cont.begin(), rcont.begin(), sortFn, sz);
    }
}

// todo: fix test for std::list
#define REGISTER_TESTS(SECTION, PREFIX, FN) \
    TEST(SECTION, PREFIX ## VectorInt) { \
        test_sort<std::vector<int>>(FN); \
    } \
    TEST(SECTION, PREFIX ## DequeInt) { \
        test_sort<std::deque<int>>(FN); \
    }
 
