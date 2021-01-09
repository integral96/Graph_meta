
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"
#include "include/matrix.hpp"

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

    std::vector<bool> vec;

    std::stringstream out;
    hana::for_each(SortList0::value, [&](auto arg) {
        if (hana::at_c<1>(hana::first(arg))) {
            out << hana::at_c<0>(hana::first(arg)) <<" <-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        } else {
            out << hana::at_c<0>(hana::first(arg)) <<" <-/-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        }
        if (hana::at_c<1>(hana::second(arg))) {
            out << hana::at_c<0>(hana::second(arg)) <<" <-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        } else {
            out << hana::at_c<0>(hana::second(arg)) <<" <-/-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        }
        vec.push_back(hana::at_c<1>(hana::first(arg)));
        vec.push_back(hana::at_c<1>(hana::second(arg)));
    });
    std::cout << out.str() << std::endl;
    vec.erase(vec.begin());
    vec.erase(vec.begin());
    matrix_2<4, 4> mtx;
    for(const auto& x : vec) {
        std::cout << "(" << x << "; " << ")\n";
    }
    std::copy(vec.begin(), vec.end(), mtx.end()->begin());



    matrix_2<4, 3> mtrx{{1,2,3}, {1,2,3}, {4,2,2}, {4,2,2}};
    matrix_2<3, 4> mtrx1{{1,2,3,4}, {1,2,3,6}, {1,4,2,2}};
////    mtrx.fill(2);
    std::cout << mtx << std::endl;
    std::cout << mtrx1 << std::endl;
    std::cout << mtrx*mtrx1 << std::endl;

//    matrix_3<4, 3, 2, int> matrx3d;
//    gen_rand_matrix(matrx3d, 1,5);
//    std::cout << matrx3d << std::endl;

//    matrix_4<4, 3, 2, 5, int> matrx4d;
//    gen_rand_matrix(matrx4d, 1,5);
//    std::cout << matrx4d << std::endl;
    return 0;
}
