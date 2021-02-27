#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <functional>

#include <boost/timer/timer.hpp>

#include "include/graph.hpp"
#include "include/matrix.hpp"
#include "include/lagrange_group.hpp"
#include "include/lib.h"
#include "include/simvolic_diff.hpp"
#include "include/testBB.hpp"

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

int main()
{
    boost::timer::cpu_timer tmr;
    matrix_2<4, 4, bool> mtrx;
    std::vector<bool> vec;
    size_t i{}, j{};
    hana::for_each(SortList0::value, [&](auto arg) {
        if (hana::at_c<1>(hana::first(arg))) {
            vec.push_back(true);
            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        } else {
            vec.push_back(false);
            std::cout << hana::at_c<0>(hana::first(arg)) <<" <-/-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        }
        if (hana::at_c<1>(hana::second(arg))) {
            vec.push_back(true);
            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        } else {
            vec.push_back(false);
            std::cout << hana::at_c<0>(hana::second(arg)) <<" <-/-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        }
    });
    for(const auto& x : vec) {
//        mtrx.fill(x);
        std::cout << x << " " <<  std::endl;
    }
    for(size_t i = 0; i < 4; ++i) {
        for(size_t j = 0; j < 4; ++j) {
            std::cout << vec[i + j*4] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mtrx <<  std::endl;
    std::cout <<"TIME\t\t" <<  tmr.format();
}
