// ================================== META ================================== //
// Project:         epidesim
// Name:            meta.hpp
// Description:     Metaprogramming utilities and metafunctions
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _META_HPP_INCLUDED
#define _META_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// ================================== INDEX ================================= //
// A type wrapping a constant index
template <std::size_t Index>
struct index_type: std::integral_constant<std::size_t Index> {};

// A variable template of a constant index 
template <std::size_t Index>
inline constexpr index_type<Index> index; 
// ========================================================================== //



// ================================ META FOR ================================ //
template <std::size_t First, std::size_t Last, class>
meta_for(T&& x, F&& f) {
    
}
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _META_HPP_INCLUDED
// ========================================================================== //
