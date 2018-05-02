/*
 * main.cpp
 * Copyright (C) 2018 dershokus <lily.coder@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <tuple>
#include <string>
#include <iostream>

#include <boost/type_index.hpp>

template <typename T, class ...Item>
struct concat;

template <typename ...Args, class ...Item>
struct concat<std::tuple<Args...>, Item...>
{
    using type = std::tuple<Args..., Item...>;
};


template <std::size_t I>
struct c
{
    static const constexpr std::size_t i = I;
};

using c1 = c<1>;
using c2 = c<2>;
using c3 = c<3>;
using c4 = c<4>;
using c5 = c<5>;
using c6 = c<6>;
using c7 = c<7>;

template <typename I1, class I2, class U = void>
struct sort;

template <typename I1, class I2>
struct sort<I1, I2, typename std::enable_if<I1::i <= I2::i>::type>
{
    using T1 = I1;
    using T2 = I2;
};

template <typename I1, class I2>
struct sort<I1, I2, typename std::enable_if<(I1::i > I2::i)>::type>
{
    using T1 = I2;
    using T2 = I1;
};


template <typename Head, typename Enable, class ...Args>
struct a
{
    using type = void;
};


template <typename Head, class Item1, class Item2, class ...Tail>
struct a<Head, typename std::enable_if<sizeof...(Tail) != 0>::type, Item1, Item2, Tail...>
{
    using next = a<
        typename concat<Head, typename sort<Item1, Item2>::T1>::type,
        void,
        typename sort<Item1, Item2>::T2,
        Tail...>;
    using type = typename next::type;

    static const constexpr bool sorted = next::sorted && std::is_same<typename sort<Item1, Item2>::T1, Item1>::value;
};

template <typename Head, class Item1, class Item2, class ...Tail>
struct a<Head, typename std::enable_if<sizeof...(Tail) == 0>::type, Item1, Item2, Tail...>
{
    using type = typename concat<Head, typename sort<Item1, Item2>::T1, typename sort<Item1, Item2>::T2>::type;

    static const constexpr bool sorted = std::is_same<typename sort<Item1, Item2>::T1, Item1>::value;
};


template <bool Sorted, typename Head, class ...Args>
struct A;


template <typename Head, class ...Args>
struct A<true, Head, Args...>
{
    using type = concat<Head, Args...>;
};

template <bool Sorted, typename T>
struct resort;

template <bool Sorted, typename ...Args>
struct resort<Sorted, std::tuple<Args...>>
{
    using type = typename A<Sorted, std::tuple<>, Args...>::type;
};

template <typename Head, class ...Args>
struct A<false, Head, Args...>
{
    using tmp = a<Head, void, Args...>;
    using type = typename resort<tmp::sorted, typename tmp::type>::type;
};


template <class ...Args>
using bsort = A<false, std::tuple<>, Args...>;




int main()
{
    std::cout << boost::typeindex::type_id<bsort<c7, c6, c5, c4, c3, c2, c1>::type>().pretty_name() << std::endl;

    return 0;
}

