
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"

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
    std::stringstream out;
    hana::for_each(SortList0::value, [&](auto arg) {
        if (hana::at_c<1>(hana::first(arg)))
            out << hana::at_c<0>(hana::first(arg)) <<" <-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        else
            out << hana::at_c<0>(hana::first(arg)) <<" <-/-> " << hana::at_c<2>(hana::first(arg)) <<"\n";
        if (hana::at_c<1>(hana::second(arg)))
            out << hana::at_c<0>(hana::second(arg)) <<" <-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
        else
            out << hana::at_c<0>(hana::second(arg)) <<" <-/-> " << hana::at_c<2>(hana::second(arg)) <<"\n";
    });
    std::cout << out.str() << std::endl;

    std::vector<std::pair<bool, bool>> vec;

    hana::for_each(SortList0::value, [&](auto arg) mutable {
        vec.push_back(std::make_pair(hana::at_c<1>(hana::first(arg)), hana::at_c<1>(hana::second(arg))));
    });
    vec.erase(vec.begin());
    for(const auto& [x, y] : vec) {
        std::cout << "(" << x << "; " << y <<")\n";
    }
    return 0;
}
