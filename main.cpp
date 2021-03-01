
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <functional>
#include <thread>

#include <boost/timer/timer.hpp>

#include "include/graph.hpp"
#include "include/matrix.hpp"
#include "include/lagrange_group.hpp"
#include "include/lib.h"
#include "include/simvolic_diff.hpp"
#include "include/testBB.hpp"

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
auto gsolve_(const Func1& fx, const Func2& dfx, double x0) {
    double x1  = x0 - fx(x0)/dfx(x0); // первое приближение

    while (std::abs(x1-x0)>eps) { // пока не достигнута точность 0.000001
      x0 = x1;
      x1 = x0 - fx(x0)/dfx(x0); // последующие приближения
    }
    return x1;
}

int main()
{
    boost::timer::cpu_timer tmr;
    std::cout << "build: " << version() <<std::endl;



//    matrix_2<4, 3, int> mtrx{{1,2,3}, {1,2,3}, {4,2,2}, {4,2,2}};
//    matrix_2<3, 4, int> mtrx1{{1,2,3,4}, {1,2,3,6}, {1,4,2,2}};

//    std::cout << mtrx1 << std::endl;
//    std::cout << mtrx*mtrx1 << std::endl;

//    matrix_2<6, 6, double> BB;
//    gen_rand_matrix(BB, 1.1, 50.5);
//    std::cout << BB << std::endl;
//    std::cout << BB*(BB^BB) << std::endl;

//    matrix_3<4, 3, 2, int> matrx3d;
//    gen_rand_matrix(matrx3d, 1,5);
//    std::cout << matrx3d << std::endl;
//
//    matrix_4<4, 3, 2, 5, int> matrx4d;
//    gen_rand_matrix(matrx4d, 1,5);
//    std::cout << matrx4d << std::endl;

//    ////Lagrange
//    ///
////Lagrange
///
//    boost::array<double, 5> x;
//    x[0] = 1;
//    x[1] = 3;
//    x[2] = 5;
//    x[3] = 7;
//    x[4] = 9;

//    boost::array<double, 5> y;
//    y[0] = 0;
//    y[1] = 2;
//    y[2] = -1;
//    y[3] = 1;
//    y[4] = 3;

//    boost::array<double, 50> x_orig;
//    boost::array<double, 50> y_orig;
//    for(int i = 0; i < 50; ++i) {
//        x_orig[i] = double(i);
//    }

//    for (double i = 0.; i < 25; i += .25) {
//        std::cout << i << "\t| " << LAGRANGE_RESULT<5>(i, .0, x, y) << std::endl;
//    }
//    std::cout << "=====================" << std::endl;
//    double dx = 1;

//    for (double i = 0.; i < 25; i += .25) {
//        std::cout << i << "\t| " << LAGRANGE_RESULT<5>(i, dx, x, y) << std::endl;
//    }
//    std::cout << "MEta =====================" << std::endl;
//    LAGRANGE_RESULT_M<5, 50>(x_orig, .0, y_orig, x, y);
//    for(int i = 0; i < 50; ++i) {
//        std::cout << x_orig[i] << "\t| " << y_orig[i] << std::endl;
//    }
    std::cout << "Random =====================" << std::endl;
    LAGRANG_SET<10, 50> result_lag(-16., 16., -16., 16., 0.);
    for(const auto& x : result_lag.get_RES()) {
        std::cout << x.first << "\t| " << x.second << std::endl;
    }


//    ///SIMvolic Diff
//    ///
//    std::stringstream ss2;
//    hana::for_each(result, [&](auto x) {
//        if(hana::first(x) + hana::second(x) == 9)
//        ss2 << hana::first(x) << " " << hana::second(x) << "\n";
//    });

//    std::stringstream ss3;
//    hana::for_each(result2, [&](auto x) {
////        if(hana::first(x) < hana::second(x))
//        ss3 << hana::first(x) << " " << hana::second(x) << "\n";
//    });

//    std::cout << ss2.str() << std::endl;
//    std::cout << "============================" << std::endl;
//    std::cout << ss3.str() << std::endl;
//    std::cout << "============================" << std::endl;
//    std::cout << U_st<3, 0>::first << std::endl;


//    variable xx;
//    double result = newton(xx*xx + xx , 1.1, 1000);
//    std::cout << result << '\n';
//    std::cout << "============================" << std::endl;
//    std::cout << solve_(fx_, dfx_, 1.1) << '\n';

//    ///Coroutine
//    matrix_2<12, 12, double> AA1, AA2, AA3, AA4;
//    gen_rand_matrix(AA1, 1.1, 50.5);
//    gen_rand_matrix(AA2, 10.1, 50.5);
//    gen_rand_matrix(AA3, 15.1, 50.5);
//    gen_rand_matrix(AA4, 5.1, 100.5);
//    std::vector<matrix_2<12, 12, double>> vec_mtrx{AA1, AA2, AA3, AA4};
//    pull_type<matrix_2<12, 12, double>> great_func(
//                std::bind(getNextElement<matrix_2<12, 12, double>>, std::placeholders::_1, std::cref(vec_mtrx)));
//    while(great_func) {
//        std::cout << great_func.get() << std::endl;
//        great_func();
//    }
    std::cout <<"TIME\t\t" <<  tmr.format();
    return 0;
}
