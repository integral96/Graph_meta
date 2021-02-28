#pragma once

#include <boost/shared_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/array.hpp>

#include <iostream>

#include "base_func.hpp"


template <size_t K, size_t N, class Array>
struct PHI_COEF /// i == 0,...,K // i == K + 1, ..., N
{
    using value_type = typename Array::value_type;
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
    using value_type = typename Array::value_type;
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
    using value_type = typename Array::value_type;
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

///Version gibrid
template<class Array>
struct RESULT {

    using value_type = typename Array::value_type;
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
typename Array::value_type LAGRANGE_RESULT(typename Array::value_type x_orig, typename Array::value_type shift,
    const Array& x_index, const Array& y) {
    RESULT<Array> closure(x_orig, shift, x_index, y);
    return closure.template value<N>();
}
///Version meta
template<size_t N, class Array, class Array_res>
struct RESULT_M {
    static_assert (std::is_same_v<typename Array::value_type, typename Array_res::value_type>, " не совпадение типов");
    using value_type = typename std::enable_if_t<std::is_same_v<typename Array::value_type, typename Array_res::value_type>,
                                typename Array::value_type>;
    const Array& y_index;
    const Array& x_index;
    Array_res& x_orig;
    Array_res& y_result;
    value_type shift;
    RESULT_M(Array_res& x_orig, value_type shift, Array_res& y_result, const Array& x_index, const Array& y_index) :
        x_index(x_index), y_index(y_index), x_orig(x_orig), shift(shift), y_result(y_result) {}
    template<size_t I>
    void apply() {
        LAGRANGE_MEM<N, Array> closure(y_index, x_orig[I] + shift, x_index);
        y_result[I] = abstract_sums<N>(closure);
    }
};
template<size_t N, size_t M, class Array, class Array_res,
         typename = std::enable_if_t<std::is_same_v<typename Array::value_type, typename Array_res::value_type>>>
void LAGRANGE_RESULT_M(Array_res& x_orig, typename Array::value_type shift, Array_res& y_result, const Array& x_index, const Array& y_index) {
    RESULT_M<N, Array, Array_res> closure(x_orig, shift, y_result, x_index, y_index);
    meta_loop<M>(closure);
}

template<size_t N>
class coordinates_set  : boost::noncopyable {
public:
    using value_type = double;
    using type_array = boost::array<value_type, N>;
    using type_pair  = boost::array<std::pair<value_type, value_type>, N>;
private:
    value_type X_range_min, Y_range_min, X_range_max, Y_range_max;
    type_array X;
    type_array Y;
    type_pair XY;
public:
    explicit coordinates_set(value_type x_min, value_type x_max, value_type y_min, value_type y_max) :
                                X_range_min(x_min), X_range_max(x_max), Y_range_min(y_min), Y_range_max(y_max) {}
    void init_X() {
        gen_rand_array<N, value_type, type_array>(X, X_range_min, X_range_max);
    }
    void init_Y() {
        gen_rand_array<N>(Y, Y_range_min, Y_range_max);
    }
    void init_XY() {
        for(size_t i = 0; i < N; ++i) {
            XY[i] = std::make_pair(X[i], Y[i]);
        }
    }
    //
    const type_array& get_X() const {
        return X;
    }
    const type_array& get_Y() const {
        return Y;
    }
    const type_pair& get_XY() const {
        return XY;
    }
    //print
    void print() {
        std::cout << "X = {";
        for(const auto& x : X) {
            std::cout << x << ", ";
        }
        std::cout << "}" << std::endl;
        std::cout << "Y = {";
        for(const auto& y : Y) {
            std::cout << y << ", ";
        }
        std::cout << "}" << std::endl;
        std::cout << "XY_set = {";
        for(const auto& xy : XY) {
            std::cout << "(" << xy.first << ", " << xy.second << "), ";
        }
        std::cout << "}" << std::endl;
    }
};
