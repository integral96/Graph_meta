#ifndef LAGRANGE_GROUP_HPP
#define LAGRANGE_GROUP_HPP

#include <boost/shared_array.hpp>
#include <boost/static_assert.hpp>

#include <iostream>

#include "base_func.hpp"

/*!
 * struct abstract_mult
 */
template<class Closure>
struct PHI_MULT {
    using value_type = typename Closure::value_type;
    PHI_MULT(Closure &closure) : closure(closure), result(value_type(1)){}
    template<size_t I>
    void apply(){
        result *= closure.template value<I>();
    }
    Closure &closure;
    value_type result;
};
template<size_t K, class Closure>
typename Closure::value_type PHI_MULT_K(Closure &closure) {
    PHI_MULT<Closure> my_closure(closure);
    meta_loop<K>(my_closure);
    return my_closure.result;
}

template<typename T>
struct Polinom_PHI {
    using value_type = T;
    T x_orig;
    boost::shared_array<T> x_index;
};

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
        if constexpr(I < K || I >= K + 1)
            return (x_orig - x_index[I])/(x_index[K] - x_index[I]);
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
    LAGRANGE_PHI(value_type x_orig, const Array& x_index) : x_orig(x_orig), x_index(x_index)   {}
    void apply() {
        PHI_COEF<K, N, Array> closure_K(x_orig, x_index);
        result = PHI_MULT_K<N>(closure_K);
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
        return y_[K]*closure.result;
    }
};

template<size_t N, class Array>
struct RESULT {

    using value_type = typename Array::element_type;
    const Array& y_;
    const Array& x_orig;
    Array& result;
    value_type shift;
    RESULT(const Array& x_orig, value_type shift, const Array& y, Array& result) :
                    y_(y), x_orig(x_orig), shift(shift), result(result) {}
    template<size_t I>
    void apply(){
        LAGRANGE_MEM<N, Array> closure(y_, x_orig[I] + shift, x_orig);
        result[I] = abstract_sums<N>(closure);
    }
};
template<size_t N, class Array>
void LAGRANGE_RESULT(const Array& x_orig, typename Array::element_type shift, const Array& y, Array& result) {
    RESULT<N, Array> my_closure(x_orig, shift, y, result);
    meta_loop<N>(my_closure);
}


#endif // LAGRANGE_GROUP_HPP
