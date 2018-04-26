# Algs

A collection of algortithms, their tests and (in future) benchmarks. Mostly for studying of algorithms than for using in real projects.

## Contents

* [Installation](https://github.com/artemeknyazev/algs#installation)
* [Testing](https://github.com/artemeknyazev/algs#testing)
* [Algorithms](https://github.com/artemeknyazev/algs#algorithms)

## Installation

```shell
> git clone --recursive https://github.com/artemeknyazev/algs
```

## Testing

```shell
# build and run tests
> make run
```

Uses [`googletest`](https://github.com/google/googletest) for testing as a submodule.

## Algorithms

### Sort

* [Selection](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/selection.hpp)
* [Insertion](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/insertion.hpp)
* [Shell](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/shell.hpp)
* [Container shuffling](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/shuffle.hpp)
* [Heaps](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/heap.hpp) — binary max heap: sift up/down, `make_heap`, heap sort
* [Merge](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/merge.hpp) — merges (general, using tmp container, in-place), merge sort (recursive and bottom-up)
* [Quicksort](https://github.com/artemeknyazev/algs/blob/master/include/algs/sort/quicksort.hpp) — partitioning (normal and dual-pivot), quicksort and select routine

### Trees

* [BST (recursive)](https://github.com/artemeknyazev/algs/blob/master/include/algs/tree/bst_recursive.hpp) — insert (normal, into root, splay), find, remove (Hibbard deletion), select k-th, rotations, in-order iteration
* [AVL](https://github.com/artemeknyazev/algs/blob/master/include/algs/tree/avl.hpp) — only insert (for now)
