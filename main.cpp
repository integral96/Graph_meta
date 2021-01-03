
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "include/graph.hpp"

static constexpr size_t N = 4;

using List = mpl::vector16<EDGE<'A', 'A', false>, EDGE<'A', 'B', true>, EDGE<'A', 'C', false>, EDGE<'A', 'D', true>,
                           EDGE<'B', 'B', false>, EDGE<'B', 'C', true>, EDGE<'B', 'D', false>, EDGE<'B', 'A', true>,
                           EDGE<'C', 'C', false>, EDGE<'C', 'D', true>, EDGE<'C', 'A', false>, EDGE<'C', 'B', true>,
                           EDGE<'D', 'D', false>, EDGE<'D', 'A', true>, EDGE<'D', 'B', false>, EDGE<'D', 'C', true>>;

using List1 = std::tuple<EDGE<'A', 'A', false>, EDGE<'A', 'B', true>, EDGE<'A', 'C', false>, EDGE<'A', 'D', true>,
                         EDGE<'B', 'B', false>, EDGE<'B', 'C', true>, EDGE<'B', 'D', false>, EDGE<'B', 'A', true>,
                         EDGE<'C', 'C', false>, EDGE<'C', 'D', true>, EDGE<'C', 'A', false>, EDGE<'C', 'B', true>,
                         EDGE<'D', 'D', false>, EDGE<'D', 'A', true>, EDGE<'D', 'B', false>, EDGE<'D', 'C', true>>;

int main()
{
    std::stringstream out;
    hana::for_each(GRAPH<N*N, List>::type::value, [&](auto arg) {
        if (hana::second(arg))
            out << hana::first(hana::first(arg)) <<" <-> " << hana::second(hana::first(arg)) <<"\n";
        else
            out << hana::first(hana::first(arg)) <<" <-/-> " << hana::second(hana::first(arg)) <<"\n";
    });
    std::cout << out.str() << std::endl;

    std::stringstream out1;
    hana::for_each(GRAPH<N*N, List1>::type::value, [&](auto arg) {
        if (hana::second(arg))
            out1 << hana::first(hana::first(arg)) <<" <-> " << hana::second(hana::first(arg)) <<"\n";
        else
            out1 << hana::first(hana::first(arg)) <<" <-/-> " << hana::second(hana::first(arg)) <<"\n";
    });
    std::cout << out1.str() << std::endl;

    bool** matrix = new bool*[N];
    for(size_t i = 0; i < N; ++i)
        matrix[i] = new bool[N];

    calc_matrix<N, List>(matrix);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j)
            std::cout << std::boolalpha << matrix[i][j] << " ";
        std::cout << std::endl;
    }

    for (size_t i = 0; i < N; ++i)
        delete [] matrix[i];
    delete [] matrix;

    return 0;
}
