#pragma once

#include <boost/shared_array.hpp>
#include <boost/static_assert.hpp>

#include <iostream>

#include "base_func.hpp"


template <size_t K, size_t N, class Array>
struct PHI_COEF /// i == 0,...,K // i == K + 1, ..., N
{
    using value_type = typename Array::element_type;
private:
    const Array& x_index;
    value_type x_orig;
public:
    PHI_COEF(value_type x_orig, const Array& x_index) : x_orig(x_orig), x_index(x_index) {}
    template<size_t I>
    value_type value() const {
        if constexpr (I < K || I >= K + 1)
            return (x_orig - x_index[I]) / (x_index[K] - x_index[I]);
        else
            return value_type(1);
    }
};

template <size_t K, size_t N, class Array>
class LAGRANGE_PHI
{
    using value_type = typename Array::element_type;
    value_type x_orig;
    const Array& x_index;
public:
    LAGRANGE_PHI(value_type x_orig, const Array& x_index) : x_orig(x_orig), x_index(x_index) {}
    void apply() {
        PHI_COEF<K, N, Array> closure_K(x_orig, x_index);
        result = abstract_multiple<N>(closure_K);
    }
    value_type result;
};

template <size_t N, class Array>
struct LAGRANGE_MEM
{
    using value_type = typename Array::element_type;
private:
    const Array& y_;
    value_type x_orig;
    const Array& x_index;
public:
    LAGRANGE_MEM(const Array& y, value_type x_orig, const Array& x_index) : y_(y), x_orig(x_orig), x_index(x_index) {}
    template<size_t K>
    value_type value() const {
        LAGRANGE_PHI<K, N, Array> closure(x_orig, x_index);
        closure.apply();
        return y_[K] * closure.result;
    }
};

template<class Array>
struct RESULT {

    using value_type = typename Array::element_type;
    const Array& y_;
    const Array& x_index;
    value_type x_orig;
    value_type shift;
    RESULT(value_type x_orig, value_type shift, const Array& x_index, const Array& y) :
        x_index(x_index), y_(y), x_orig(x_orig), shift(shift) {}
    template<size_t N>
    value_type value() {
        LAGRANGE_MEM<N, Array> closure(y_, x_orig + shift, x_index);
        return abstract_sums<N>(closure);
    }
};
template<size_t N, class Array>
typename Array::element_type LAGRANGE_RESULT(typename Array::element_type x_orig, typename Array::element_type shift,
    const Array& x_index, const Array& y) {
    RESULT<Array> my_closure(x_orig, shift, x_index, y);
    return my_closure.template value<N>();
}

