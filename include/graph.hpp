#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <type_traits>
#include <boost/hana.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/at.hpp>
#include <tuple>

namespace hana = boost::hana;
namespace mpl = boost::mpl;

template <size_t N, size_t I, class Closure>
typename std::enable_if_t<(I == N)> is_meta_loop(Closure& closure) {}

template <size_t N, size_t I, class Closure>
typename std::enable_if_t<(I < N)> is_meta_loop(Closure& closure) {
    closure.template apply<I>();
    is_meta_loop<N, I + 1>(closure);
}
template <size_t N, class Closure>
void meta_loop(Closure& closure) {
    is_meta_loop<N, 0>(closure);
}
/////
///
///


template<char A, char B, bool predicat>
struct EDGE {
    static constexpr auto value = hana::make_pair(hana::make_pair(A, B), predicat);
};

template<typename ... Edges>
struct EDGE_LIST {
    static constexpr auto value = hana::make_tuple(Edges::value...);
};

template<typename List>
struct is_Empty : mpl::false_ {};
template<>
struct is_Empty<EDGE_LIST<>> : mpl::true_ {};

//Front
template<typename List> struct FrontT;
template<typename Head, typename ... Tail>
struct FrontT<EDGE_LIST<Head, Tail...>> {
    using type = Head;
};
template<typename List>
using Front = typename FrontT<List>::type;

//Pop
template<typename List> struct PopFrontT;
template<typename Head, typename ... Tail>
struct PopFrontT<EDGE_LIST<Head, Tail...>> {
    using type = EDGE_LIST<Tail...>;
};
template<typename List>
using PopFront = typename PopFrontT<List>::type;
//PushFront
template<typename List, typename NewElem> struct PushFrontT;
template<typename... Elements, typename NewElem>
struct PushFrontT<EDGE_LIST<Elements...>, NewElem> {
    using type = EDGE_LIST<NewElem, Elements...>;
};
template<typename List, typename NewElem>
using PushFront = typename PushFrontT<List, NewElem>::type;

template<typename List, typename NewElem>
struct PushBackT;

template<typename... Elements, typename NewElem>
struct PushBackT<EDGE_LIST<Elements...>, NewElem> {
    using type = EDGE_LIST<Elements..., NewElem>;
};

template<typename List, typename NewElem>
using PushBack = typename PushBackT<List, NewElem>::type;

template<typename T, typename ...Args>
struct is_tuple : mpl::false_ {};
template<typename ...Args>
struct is_tuple<std::tuple<Args...>> : mpl::true_ {};

template<size_t N, class List>
struct GRAPH_tuple {
    using type = PushBack<typename GRAPH_tuple<N - 1, List>::type,
    typename std::tuple_element_t<N - 1, List>>;
};
template<class List>
struct GRAPH_tuple<0, List> {
    using type = PushBack<EDGE_LIST<>,
    typename std::tuple_element_t<0, List>>;
};

template<size_t N, class List>
struct GRAPH_mpl {
    using type = PushBack<typename GRAPH_mpl<N - 1, List>::type,
    typename mpl::at_c<List, N - 1>::type>;
};
template<class List>
struct GRAPH_mpl<0, List> {
    using type = PushBack<EDGE_LIST<>,
    typename mpl::at_c<List, 0>::type>;
};


template<size_t N, class List>
struct GRAPH : mpl::if_c<is_tuple<List>::value, GRAPH_tuple<N, List>,
               GRAPH_mpl<N, List>>::type {};

//Matrix incede


template<size_t I, size_t J, size_t N, class List>
struct MATRIX_INC {
    using T = typename mpl::at_c<List, I*N + J>::type;
};

template <size_t I, size_t N, typename List, typename Matrix>
struct INIT_MEMBER {
private:
    Matrix& C;
public:
    INIT_MEMBER(Matrix& C): C(C) {}
    template<size_t J>
    void apply() const {
        using loop_type = MATRIX_INC<I, J, N - 1, List>;
        constexpr auto a = hana::first(loop_type::T::value);
        constexpr auto b = hana::second(loop_type::T::value);
            C[I][J] = b;
    }
};

template <size_t N, typename List, typename Matrix>
struct INIT_MATRIX_1
{
private:
    Matrix& C;
public:
    INIT_MATRIX_1(Matrix& C): C(C) {}
    template <size_t I>
    void apply() {
        INIT_MEMBER<I, N, List, Matrix> closure(C);
        meta_loop<N>(closure);
    }
};
template <size_t N, typename List, typename Matrix>
inline void calc_matrix(Matrix& C) {
    INIT_MATRIX_1<N, List, Matrix> closure(C);
    meta_loop<N>(closure);
}

#endif // GRAPH_HPP

