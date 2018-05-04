#pragma once
#include <tuple>

/**
 * Add to a tuple type new element types.
 */
template <typename T, class ...Item>
struct concatenate;

template <typename ...Args, class ...Item>
struct concatenate<std::tuple<Args...>, Item...>
{
    using type = std::tuple<Args..., Item...>;
};

/**
 * One iteration of the sorting algorithm.
 * It checks is a sequence sorted after the iteration.
 */
template <typename Head, template<class, class> class Cmp, typename Enable, class ...Args>
struct selection_sort_impl;

template <typename Head, template<class, class> class Cmp, class Item1, class Item2, class ...Tail>
struct selection_sort_impl<Head, Cmp, typename std::enable_if<sizeof...(Tail) != 0>::type, Item1, Item2, Tail...>
{
    using next = selection_sort_impl<
        typename concatenate<Head, typename Cmp<Item1, Item2>::T1>::type,
        Cmp,
        void,
        typename Cmp<Item1, Item2>::T2,
        Tail...>;
    using type = typename next::type;

    static const constexpr bool sorted = next::sorted && std::is_same<typename Cmp<Item1, Item2>::T1, Item1>::value;
};

template <typename Head, template<class, class> class Cmp, class Item1, class Item2, class ...Tail>
struct selection_sort_impl<Head, Cmp, typename std::enable_if<sizeof...(Tail) == 0>::type, Item1, Item2, Tail...>
{
    using type = typename concatenate<Head, typename Cmp<Item1, Item2>::T1, typename Cmp<Item1, Item2>::T2>::type;

    static const constexpr bool sorted = std::is_same<typename Cmp<Item1, Item2>::T1, Item1>::value;
};

/**
 * TODO
 */
template <bool Sorted, template<class, class> class Compare, typename Head, class ...Args>
struct sort_if_needed;


template <template<class, class> class Compare, typename Head, class ...Args>
struct sort_if_needed<true, Compare, Head, Args...>
{
    using type = typename concatenate<Head, Args...>::type;
};

/**
 * TODO
 */
template <bool Sorted, template<class, class> class Compare, typename T>
struct resort;

template <bool Sorted, template<class, class> class Compare, typename ...Args>
struct resort<Sorted, Compare, std::tuple<Args...>>
{
    using type = typename sort_if_needed<Sorted, Compare, std::tuple<>, Args...>::type;
};

template <template<class, class> class Compare, typename Head, class ...Args>
struct sort_if_needed<false, Compare, Head, Args...>
{
private:
    using partial_sort = selection_sort_impl<Head, Compare, void, Args...>;
public:
    using type = typename resort<partial_sort::sorted, Compare, typename partial_sort::type>::type;
};

/**
 * Additional specializations for one argument and for empty parameters list.
 */
template<template<class, class> class Compare, typename Head, class T>
struct sort_if_needed<false, Compare, Head, T>
{
    using type = std::tuple<T>;
};

template<template<class, class> class Compare, typename Head>
struct sort_if_needed<false, Compare, Head>
{
    using type = std::tuple<>;
};

/**
 * The main interface.
 */
template <template<class, class> class Compare, class ...Args>
using selection_sort = sort_if_needed<false, Compare, std::tuple<>, Args...>;

