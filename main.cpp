#include <cstddef>

#include "type_sort.hpp"

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
struct less_impl;

template <typename I1, class I2>
struct less_impl<I1, I2, typename std::enable_if<I1::i <= I2::i>::type>
{
    using T1 = I1;
    using T2 = I2;
};

template <typename I1, class I2>
struct less_impl<I1, I2, typename std::enable_if<(I1::i > I2::i)>::type>
{
    using T1 = I2;
    using T2 = I1;
};

template <class T1, class T2>
using less = less_impl<T1, T2>;



int main()
{
    static_assert(std::is_same<selection_sort<less, c7, c6, c5, c4, c3, c2, c1>::type, std::tuple<c1, c2, c3, c4, c5, c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7, c6, c5, c4, c3, c2>::type, std::tuple<c2, c3, c4, c5, c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7, c6, c5, c4, c3>::type, std::tuple<c3, c4, c5, c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7, c6, c5, c4>::type, std::tuple<c4, c5, c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7, c6, c5>::type, std::tuple<c5, c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7, c6>::type, std::tuple<c6, c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less, c7>::type, std::tuple<c7 >>::value, "");
    static_assert(std::is_same<selection_sort<less>::type, std::tuple<>>::value, "");

    return 0;
}

