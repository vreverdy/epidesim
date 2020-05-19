// ================================== BASE ================================== //
// Project:         epidesim
// Name:            base.hpp
// Description:     Generic bases to help with the creation of common types
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BASE_HPP_INCLUDED
#define _BASE_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <tuple>
#include <vector>
#include <iterators>
// Project sources
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //

children
parent

preorder_dfs
postorder_dfs
inorder_dfs

depth_first_search
breadth_first_search



/* ********************************** TREE ********************************** */
// A basic tree data structure
template <class Type, class Container = std::vector<Type>>
class tree
{
    // Types
    public:
    using value_type = Type;
    using container_type = Container;
    class node;
    template <class>
    class iterator;
};
/* ************************************************************************** */



/* ********************************** NODE ********************************** */
// The node type of a tree
template <class Type, class Container>
class tree<Type, Container>::node
{
    // Types
    public:
    using tree_type = tree<Type, Container>;
    using value_type = typename tree_type::value_type;
    using container_type = typename tree_type::container_type;

    public:
    constexpr value_type&

    template<class T>
    constexpr begin<T>
    
    //
    public:
    value_type _value;
    
};
/* ************************************************************************** */



/* ******************************** ITERATOR ******************************** */
template <class Type, class Container>
class tree<Type, Container>::iterator<children>
{
    
};

template <class Type, class Container>
class tree<Type, Container>::iterator<ancestors>
{
    
};

template <class Type, class Container>
class tree<Type, Container>::iterator<siblings>
{
    
};

template <class Type, class Container>
class tree<Type, Container>::iterator<breadth_first_search>
{
};

template <class Type, class Container>
class tree<Type, Container>::iterator<depth_first_search<pre_order>>
{
};

template <class Type, class Container>
template <std::size_t N>
class tree<Type, Container>::iterator<depth_first_search<in_order<N>>>
{
};

template <class Type, class Container>
class tree<Type, Container>::iterator<depth_first_search<post_order>>
{
};
/* ************************************************************************** */



// ========================================================================== //
} // namespace epidesim
#endif // _TREE_HPP_INCLUDED
// ========================================================================== //
