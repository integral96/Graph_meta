
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"
#include "include/matrix.hpp"
#include "include/lagrange_group.hpp"
#include "include/lib.h"
#include "include/simvolic_diff.hpp"

////Simvolic
constexpr auto A = hana::make_set(hana::int_c<1>, hana::int_c<3>, hana::int_c<5>, hana::int_c<7>);
constexpr auto B = hana::make_set(hana::int_c<2>, hana::int_c<4>, hana::int_c<6>);

constexpr auto U = hana::union_(A, B);

constexpr auto result = hana::unpack(U, [](auto... x){
    return hana::make_set(hana::make_pair(9 - x, hana::max(x, 9 - x) == x ? x : 0)...);
});
constexpr auto result1 = hana::unpack(U, [](auto... x){
    return hana::make_tuple(hana::make_pair(x,  8 - x)...);
});

using type_A = mpl::vector4_c<int, 1, 3, 5, 7>;
using type_B = mpl::vector3_c<int, 2, 4, 6>;

template<int N, int M>
struct pair {

    static constexpr int first = mpl::at_c<type_A, N>::type::value;
    static constexpr int second = mpl::at_c<type_B, M>::type::value;
};

template<int N, int M>
struct U_st : mpl::if_c<(mpl::plus<typename mpl::at_c<type_A, N>::type, typename mpl::at_c<type_B, M>::type>::type::value == 9),
                        pair<N, M>, mpl::na>::type {

};
auto result2 = hana::sort(result1, hana::less);

///Graph
static constexpr size_t N = 10;

using List = mpl::vector<EDGE<'A', false, 'A', false>, EDGE<'A', true, 'B', true>,   EDGE<'A', false, 'C', false>, EDGE<'A', true, 'D', true>,
                         EDGE<'C', true, 'D', true>,   EDGE<'B', false, 'D', false>, EDGE<'B', true, 'C', true>,
                         EDGE<'B', false, 'B', false>, EDGE<'C', false, 'C', false>, EDGE<'D', false, 'D', false>>;

using List1 = std::tuple<EDGE<'A', false, 'A', false>, EDGE<'A', true, 'B', true>,   EDGE<'A', false, 'C', false>, EDGE<'A', true, 'D', true>,
                         EDGE<'C', true, 'D', true>,   EDGE<'B', false, 'D', false>, EDGE<'B', true, 'C', true>,
                         EDGE<'B', false, 'B', false>, EDGE<'C', false, 'C', false>, EDGE<'D', false, 'D', false>>;

using List2 = fusion::tuple<EDGE<'A', false, 'A', false>, EDGE<'A', true, 'B', true>,   EDGE<'A', false, 'C', false>, EDGE<'A', true, 'D', true>,
                            EDGE<'C', true, 'D', true>,   EDGE<'B', false, 'D', false>, EDGE<'B', true, 'C', true>,
                            EDGE<'B', false, 'B', false>, EDGE<'C', false, 'C', false>, EDGE<'D', false, 'D', false>>;

template<typename T1, typename T2>
struct SmallThenT {
    static constexpr bool value = hana::lexicographical_compare(std::declval<T1>(), std::declval<T2>());
};
using SortList0 = InsertionSort<GRAPH<N, List>::type, SmallThenT>;
using SortList1 = InsertionSort<GRAPH<N, List1>::type, SmallThenT>;
using SortList2 = InsertionSort<GRAPH<N, List2>::type, SmallThenT>;

///Simvolic classic
///
#define eps 0.000001

static auto fx_([](auto x) ->double { // вычисляемая функция
    return x*x + x;
});
static auto dfx_([](auto x) ->double { // производная функции
    return 2*x + 1;
});

template<class Func1, class Func2>
auto solve_(const Func1& fx, const Func2& dfx, double x0) {
    double x1  = x0 - fx(x0)/dfx(x0); // первое приближение

    while (std::abs(x1-x0)>eps) { // пока не достигнута точность 0.000001
      x0 = x1;
      x1 = x0 - fx(x0)/dfx(x0); // последующие приближения
    }
    return x1;
}

int main()
{
    std::cout << "build: " << version() <<std::endl;

    hana::for_each(SortList0::value, [&](auto arg) {
        if (hana::at_c<1>(hana::first(arg))) {
            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        } else {
            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-/-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        }
        if (hana::at_c<1>(hana::second(arg))) {
            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        } else {
            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-/-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        }
    });


    matrix_2<4, 3, int> mtrx{{1,2,3}, {1,2,3}, {4,2,2}, {4,2,2}};
    matrix_2<3, 4, int> mtrx1{{1,2,3,4}, {1,2,3,6}, {1,4,2,2}};

    std::cout << mtrx1 << std::endl;
    std::cout << mtrx*mtrx1 << std::endl;
;
    matrix_2<8, 8, double> BB;
    gen_rand_matrix(BB, 1.1, 50.5);
    std::cout << BB << std::endl;
    std::cout << BB*(BB^BB) << std::endl;

//    matrix_3<4, 3, 2, int> matrx3d;
//    gen_rand_matrix(matrx3d, 1,5);
//    std::cout << matrx3d << std::endl;
//
//    matrix_4<4, 3, 2, 5, int> matrx4d;
//    gen_rand_matrix(matrx4d, 1,5);
//    std::cout << matrx4d << std::endl;

    ////Lagrange
    ///
    boost::shared_array<double> x( new double[5]);
    x[0] = 1;
    x[1] = 3;
    x[2] = 5;
    x[3] = 7;
    x[4] = 9;

    boost::shared_array<double> y( new double[5]);
    y[0] = 0;
    y[1] = 2;
    y[2] = -1;
    y[3] = 1;
    y[4] = 3;
    boost::shared_array<double> res( new double[5]);
    boost::shared_array<double> res2( new double[5]);

    LAGRANGE_RESULT<5>(x, 0., y, res);
    for(size_t i = 0; i < 5; ++i) {
        std::cout << x[i] << "\t| " << res[i] << std::endl;
    }
    std::cout << "=====================" << std::endl;
    double dx = 1;
//    for(size_t i = 0; i < 5; ++i) x[i] += dx;
    LAGRANGE_RESULT<5>(x, dx, y, res2);
    for(size_t i = 0; i < 5; ++i) {
        std::cout << x[i] << "\t| " << res2[i] << std::endl;
    }

    ///SIMvolic Diff
    ///
    std::stringstream ss2;
    hana::for_each(result, [&](auto x) {
        if(hana::first(x) + hana::second(x) == 9)
        ss2 << hana::first(x) << " " << hana::second(x) << "\n";
    });

    std::stringstream ss3;
    hana::for_each(result2, [&](auto x) {
//        if(hana::first(x) < hana::second(x))
        ss3 << hana::first(x) << " " << hana::second(x) << "\n";
    });

    std::cout << ss2.str() << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << ss3.str() << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << U_st<3, 0>::first << std::endl;


    variable xx;
    double result = newton(xx*xx + xx , 1.1, 1000);
    std::cout << result << '\n';
    std::cout << "============================" << std::endl;
    std::cout << solve_(fx_, dfx_, 1.1) << '\n';
    return 0;
}
