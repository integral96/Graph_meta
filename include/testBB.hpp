#pragma once

#include <vector>
#include <algorithm>    //std::max
#include <boost/coroutine2/all.hpp>

namespace cor = boost::coroutines2;

template<typename T>
using pull_type = typename cor::asymmetric_coroutine<T>::pull_type;

template<typename T>
using push_type = typename cor::asymmetric_coroutine<T>::push_type;

template<typename T>
void getNextElement(push_type<T>& sink, const std::vector<T>& vec) {
    for(auto BB : vec) {
        sink(BB*(BB^BB));
    }
}
