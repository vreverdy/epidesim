// =============================== CONSTANTS ================================ //
// Project:         epidesim
// Name:            constants.hpp
// Description:     Compile time constants for metaprogramming
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _CONSTANTS_HPP_INCLUDED
#define _CONSTANTS_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <utility>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// ================================ CONSTANT ================================ //
// A generic constant
template <auto Value>
struct constant {
    using type = constant;
    using value_type = decltype(Value);
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
    static constexpr auto value = Value;
};

// Variable template
template <auto Value>
inline constexpr constant<Value> constant_v;
// ========================================================================== //



// ============================= INDEX CONSTANT ============================= //
// An index constant
template <std::size_t Index>
struct index_constant: constant<Index> {
    using integral_constant = std::integral_constant<std::size_t, Index>;
    using index_constant::constant::constant;
    constexpr index_constant(integral_constant) noexcept {
    }
    template <class T, T I, class = std::enable_if_t<I == Index>>
    explicit constexpr index_constant(std::integral_constant<T, I>) noexcept {
    }
    constexpr operator integral_constant() const noexcept {
        return integral_constant{};
    }
    template <class T, T I, class = std::enable_if_t<I == Index>>
    explicit constexpr operator std::integral_constant<T, I>() const noexcept {
        return std::integral_constant<T, I>{};
    }
    template <class T, T I, class = std::enable_if_t<I == Index>>
    constexpr index_constant& operator=(std::integral_constant<T, I>) noexcept {
        return *this;
    }
};

// Variable template
template <std::size_t Index>
inline constexpr index_constant<Index> index_v;
// ========================================================================== //



// ============================== SIZE CONSTANT ============================= //
// A size constant
template <std::size_t Size>
struct size_constant: constant<Size> {
    using integral_constant = std::integral_constant<std::size_t, Size>;
    using index_sequence = std::make_index_sequence<Size>;
    using size_constant::constant::constant;
    constexpr size_constant(integral_constant) noexcept {
    }
    template <class T, T S, class = std::enable_if_t<S == Size>>
    explicit constexpr size_constant(std::integral_constant<T, S>) noexcept {
    }
    constexpr operator integral_constant() const noexcept {
        return integral_constant{};
    }
    template <class T, T S, class = std::enable_if_t<S == Size>>
    explicit constexpr operator std::integral_constant<T, S>() const noexcept {
        return std::integral_constant<T, S>{};
    }
    template <class T, T S, class = std::enable_if_t<S == Size>>
    constexpr size_constant& operator=(std::integral_constant<T, S>) noexcept {
        return *this;
    }
};

// Variable template
template <std::size_t Size>
inline constexpr size_constant<Size> size_v;
// ========================================================================== //



// ========================= FLOATING POINT CONSTANT ======================== //
template <
    class Type,
    long long int Mantissa,
    std::size_t Base,
    long long int Exponent
>
struct floating_point_constant {
    using type = floating_point_constant;
    using value_type = Type;
    static constexpr long long int mantissa = Mantissa;
    static constexpr std::size_t base = Base;
    static constexpr long long int exponent = Exponent;
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
    static constexpr value_type convert() {
        using uint_t = unsigned long long int;
        constexpr decltype(exponent) zero = 0;
        constexpr uint_t one = 1;
        constexpr value_type m = mantissa;
        value_type r = one;
        uint_t e = exponent > zero ? exponent : -exponent;
        for (uint_t b = base; e; e >>= one, b *= b) {
            if (e & one) {
                r *= b;
            }
        }
        
        return exponent < zero ? m / r : m * r;
    }
    static constexpr value_type value = convert();
};

// Generic variable template
template <
    class Type,
    long long int Mantissa,
    std::size_t Base,
    long long int Exponent
>
inline constexpr floating_point_constant<Type, Mantissa, Base, Exponent>
floating_point_v;

// Scientific variable template
template <
    long long int Mantissa,
    long long int Exponent,
    class Type = double,
    std::size_t Base = 10
>
inline constexpr floating_point_constant<Type, Mantissa, Base, Exponent>
scientific_v;
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _CONSTANTS_HPP_INCLUDED
// ========================================================================== //
