#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <type_traits>
#include <boost/hana.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/at.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <tuple>

namespace hana = boost::hana;
namespace mpl = boost::mpl;
namespace fusion = boost::fusion;


template<char A, char B, bool predicat>
struct EDGE {
    static constexpr auto value = hana::make_pair(hana::make_pair(A, B), predicat);
};

template<typename ... Edges>
struct EDGE_LIST {
    static constexpr auto value = hana::make_tuple(Edges::value...);
};

template<typename List, typename ... Edges>
struct is_Empty : mpl::false_ {};
template<typename ... Edges>
struct is_Empty<EDGE_LIST<Edges...>> : mpl::true_ {};

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

template<typename T, typename ...Args>
struct is_fusion_tuple : mpl::false_ {};
template<typename ...Args>
struct is_fusion_tuple<fusion::tuple<Args...>> : mpl::true_ {};

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
struct GRAPH_fusion {
    using type = PushBack<typename GRAPH_fusion<N - 1, List>::type,
    typename std::decay_t<decltype (fusion::get<N - 1>(std::declval<List>()))>>;
};
template<class List>
struct GRAPH_fusion<0, List> {
    using type = PushBack<EDGE_LIST<>,
    typename std::decay_t<decltype (fusion::get<0>(std::declval<List>()))>>;
};


template<size_t N, class List>
struct GRAPH : mpl::if_c<is_tuple<List>::value, GRAPH_tuple<N, List>,
               typename mpl::if_c<is_fusion_tuple<List>::value, GRAPH_fusion<N, List>, GRAPH_mpl<N, List>>::type>::type {};

//Sortirovka


template<typename List, typename Elem, template<typename T1, typename T2> class Compare, bool = is_Empty<List>::value>
struct InsertSortedT;

template<typename List, typename Elem, template<typename T1, typename T2> class Compare>
struct InsertSortedT<List, Elem, Compare, false> {
private:
    using New_Tail = typename mpl::if_c<Compare<Elem, Front<List>>::value, mpl::identity<List>,
                              InsertSortedT<PopFront<List>, Elem, Compare>>::type;
    using New_Head = typename mpl::if_c<Compare<Elem, Front<List>>::value, Elem, Front<List>>;
public:
    using type = PushFront<New_Tail, New_Head>;
};
template<typename List, typename Elem, template<typename T1, typename T2> class Compare>
struct InsertSortedT<List, Elem, Compare, true> : PushFrontT<List, Elem> {};

template<typename List, typename Elem, template<typename T1, typename T2> class Compare>
using InsertSorted = typename InsertSortedT<List, Elem, Compare>::type;

template<typename List, template<typename T1, typename T2> class Compare, bool = is_Empty<List>::value>
struct InsertionSortT;

template<typename List, template<typename T1, typename T2> class Compare>
using InsertionSort = typename InsertionSortT<List, Compare>::type;

template<typename List, template<typename T1, typename T2> class Compare>
struct InsertionSortT<List, Compare, false> : InsertSortedT<InsertionSort<PopFront<List>, Compare>,
                                              Front<List>, Compare> {};

template<typename List, template<typename T1, typename T2> class Compare>
struct InsertionSortT<List, Compare, true> {
    using type = List;
};

#endif // GRAPH_HPP

