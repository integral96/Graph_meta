
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"
#include "include/matrix.hpp"
#include "include/lagrange_group.hpp"
#include "include/lib.h"

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

int main()
{
    std::cout << "build: " << version() <<std::endl;

//    std::vector<bool> vec;
//
////    std::stringstream out;
//    hana::for_each(SortList0::value, [&](auto arg) {
//        if (hana::at_c<1>(hana::first(arg))) {
//            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
//        } else {
//            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-/-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
//        }
//        if (hana::at_c<1>(hana::second(arg))) {
//            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
//        } else {
//            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-/-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
//        }
//        vec.push_back(hana::at_c<1>(hana::first(arg)));
//        vec.push_back(hana::at_c<1>(hana::second(arg)));
//    });
//    std::cout << out.str() << std::endl;
//    vec.erase(vec.begin());
//    vec.erase(vec.begin());
//    matrix_2<4, 4> mtx;
//    for(const auto& x : vec) {
//        std::cout << "(" << x << "; " << ")\n";
//    }
//    std::copy(vec.begin(), vec.end(), mtx.end()->begin());



//    matrix_2<4, 3, int> mtrx{{1,2,3}, {1,2,3}, {4,2,2}, {4,2,2}};
//    matrix_2<3, 4, int> mtrx1{{1,2,3,4}, {1,2,3,6}, {1,4,2,2}};
//    matrix_2<4, 4, int> AA{{1,2,3,4}, {1,2,3,6}, {1,4,2,2}, {1,4,2,2}};

//    matrix_2<5, 5, double> AA{{1, 2, 3, 4,-2},
//                                {-5, 5, 7, 8, 4},
//                                {9, 10, -11, 12, 1},
//                                {13, -14, -15, 0, 9},
//                                {20, -26, 16, -17, 25}};
////    mtrx.fill(2);
//    std::cout << mtx << std::endl;
//    std::cout << mtrx1 << std::endl;
//    std::cout << mtrx*mtrx1 << std::endl;
//    auto res = mtrx*mtrx1;
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
    return 0;
}
