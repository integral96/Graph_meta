
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"

static constexpr size_t N = 4;

using List = mpl::vector16<EDGE<'A', 'A', false>, EDGE<'A', 'B', true>,  EDGE<'A', 'C', false>, EDGE<'A', 'D', true>,
                           EDGE<'B', 'A', true>,  EDGE<'B', 'B', false>, EDGE<'B', 'C', true>,  EDGE<'B', 'D', false>,
                           EDGE<'C', 'A', false>, EDGE<'C', 'B', true>,  EDGE<'C', 'C', false>, EDGE<'C', 'D', true>,
                           EDGE<'D', 'A', true>,  EDGE<'D', 'B', false>, EDGE<'D', 'C', true>,  EDGE<'D', 'D', false>>;

using List1 = std::tuple<EDGE<'A', 'A', false>, EDGE<'A', 'B', true>,  EDGE<'A', 'C', false>, EDGE<'A', 'D', true>,
                         EDGE<'B', 'A', true>,  EDGE<'B', 'B', false>, EDGE<'B', 'C', true>,  EDGE<'B', 'D', false>,
                         EDGE<'C', 'A', false>, EDGE<'C', 'B', true>,  EDGE<'C', 'C', false>, EDGE<'C', 'D', true>,
                         EDGE<'D', 'A', true>,  EDGE<'D', 'B', false>, EDGE<'D', 'C', true>,  EDGE<'D', 'D', false>>;

using List2 = fusion::tuple<EDGE<'A', 'A', false>, EDGE<'A', 'B', true>,  EDGE<'A', 'C', false>, EDGE<'A', 'D', true>,
                            EDGE<'B', 'A', true>,  EDGE<'B', 'B', false>, EDGE<'B', 'C', true>,  EDGE<'B', 'D', false>,
                            EDGE<'C', 'A', false>, EDGE<'C', 'B', true>,  EDGE<'C', 'C', false>, EDGE<'C', 'D', true>,
                            EDGE<'D', 'A', true>,  EDGE<'D', 'B', false>, EDGE<'D', 'C', true>,  EDGE<'D', 'D', false>>;

template<typename T1, typename T2>
struct SmallThenT {
    static constexpr bool value = hana::lexicographical_compare(std::declval<T1>(), std::declval<T2>());
};
using SortList0 = InsertionSort<GRAPH<N*N, List>::type, SmallThenT>;
using SortList1 = InsertionSort<GRAPH<N*N, List1>::type, SmallThenT>;
using SortList2 = InsertionSort<GRAPH<N*N, List2>::type, SmallThenT>;

int main()
{
    std::stringstream out;
    int i{};
    int j{};
    hana::for_each(SortList1::value, [&](auto arg) {
        if (hana::second(arg))
            out << hana::first(hana::first(arg)) <<" <-> " << hana::second(hana::first(arg)) <<"\n";
        else
            out << hana::first(hana::first(arg)) <<" <-/-> " << hana::second(hana::first(arg)) <<"\n";
    });
    std::cout << out.str() << std::endl;

    std::vector<bool> vec;

    hana::for_each(SortList1::value, [&](auto arg) mutable {
        vec.push_back(hana::second(arg));
    });
    vec.erase(vec.begin());
    for(const auto& x : vec) {
        std::cout << x << " ";
        if(i == N - 1 || i == 2*N - 1 || i == 3*N - 1  ) std::cout << std::endl;
        i++;
    }
//    std::cout << /*std::boolalpha << */matrix << std::endl;
    return 0;
}
