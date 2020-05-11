// ================================= TRAITS ================================= //
// Project:         epidesim
// Name:            traits.hpp
// Description:     Type traits
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _TRAITS_HPP_INCLUDED
#define _TRAITS_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <tuple>
#include <memory>
#include <iterator>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// ============================== REMOVE CVREF ============================== //
// Removes const, volatile, and reference qualifiers of a type
template <class T>
struct remove_cvref: std::remove_cv<std::remove_reference_t<T>> {};

// Alias template
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
// ========================================================================== //



// =============================== IS ITERATOR ============================== //
// Checks if a type is an iterator: not a an iterator
template <class Iterator, class Category = void, class = void>
struct is_iterator: std::false_type {};

// Checks if a type is an iterator: all kinds of iterators
template <class Iterator>
struct is_iterator<Iterator, void, std::void_t<
    typename std::iterator_traits<Iterator>::iterator_category
>>: std::true_type {};

// Checks if a type is an iterator: iterators of a specified category
template <class Iterator, class Category>
struct is_iterator<Iterator, Category, std::void_t<
    typename std::iterator_traits<Iterator>::iterator_category
>>: std::is_base_of<
    Category,
    typename std::iterator_traits<Iterator>::iterator_category
> {};

// Variable template
template <class T, class Category = void>
inline constexpr bool is_iterator_v = is_iterator<T, Category>::value;

// Alias template
template <class T, class Category = void>
using if_iterator_t = std::enable_if_t<is_iterator_v<T, Category>>;
// ========================================================================== //



// =========================== IS OUTPUT ITERATOR =========================== //
// Checks if a type is an output iterator
template <class T>
struct is_output_iterator
: is_iterator<T, std::output_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_output_iterator_v = is_output_iterator<T>::value;

// Alias template
template <class T>
using if_output_iterator_t = std::enable_if_t<is_output_iterator_v<T>>;
// ========================================================================== //



// ============================ IS INPUT ITERATOR =========================== //
// Checks if a type is an input iterator
template <class T>
struct is_input_iterator
: is_iterator<T, std::input_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_input_iterator_v = is_input_iterator<T>::value;

// Alias template
template <class T>
using if_input_iterator_t = std::enable_if_t<is_input_iterator_v<T>>;
// ========================================================================== //



// =========================== IS FORWARD ITERATOR ========================== //
// Checks if a type is a forward iterator
template <class T>
struct is_forward_iterator
: is_iterator<T, std::forward_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_forward_iterator_v = is_forward_iterator<T>::value;

// Alias template
template <class T>
using if_forward_iterator_t = std::enable_if_t<is_forward_iterator_v<T>>;
// ========================================================================== //



// ======================== IS BIDIRECTIONAL ITERATOR ======================= //
// Checks if a type is a bidirectional iterator
template <class T>
struct is_bidirectional_iterator
: is_iterator<T, std::bidirectional_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_bidirectional_iterator_v
= is_bidirectional_iterator<T>::value;

// Alias template
template <class T>
using if_bidirectional_iterator_t
= std::enable_if_t<is_bidirectional_iterator_v<T>>;
// ========================================================================== //



// ======================== IS RANDOM ACCESS ITERATOR ======================= //
// Checks if a type is a random access iterator
template <class T>
struct is_random_access_iterator
: is_iterator<T, std::random_access_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_random_access_iterator_v
= is_random_access_iterator<T>::value;

// Alias template
template <class T>
using if_random_access_iterator_t
= std::enable_if_t<is_random_access_iterator_v<T>>;
// ========================================================================== //



// ================================ IS RANGE ================================ //
// Checks if a type is a range: not a range
template <class T, class Category = void, class = void>
struct is_range: std::false_type {};

// Checks if a type is a range: it is a range
template <class Range, class Category>
struct is_range<Range, Category, std::void_t<
    decltype(std::begin(std::declval<Range>())),
    decltype(std::end(std::declval<Range>()))
>>: is_iterator<
    remove_cvref_t<decltype(std::begin(std::declval<Range>()))>,
    Category
> {};

// Variable template
template <class T, class Category = void>
inline constexpr bool is_range_v = is_range<T, Category>::value;

// Alias template
template <class T, class Category = void>
using if_range_t = std::enable_if_t<is_range_v<T, Category>>;
// ========================================================================== //



// ============================= IS OUTPUT RANGE ============================ //
// Checks if a type is an output range
template <class T>
struct is_output_range
: is_range<T, std::output_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_output_range_v = is_output_range<T>::value;

// Alias template
template <class T>
using if_output_range_t = std::enable_if_t<is_output_range_v<T>>;
// ========================================================================== //



// ============================= IS INPUT RANGE ============================= //
// Checks if a type is an input range
template <class T>
struct is_input_range
: is_range<T, std::input_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_input_range_v = is_input_range<T>::value;

// Alias template
template <class T>
using if_input_range_t = std::enable_if_t<is_input_range_v<T>>;
// ========================================================================== //



// ============================ IS FORWARD RANGE ============================ //
// Checks if a type is a forward range
template <class T>
struct is_forward_range
: is_range<T, std::forward_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_forward_range_v = is_forward_range<T>::value;

// Alias template
template <class T>
using if_forward_range_t = std::enable_if_t<is_forward_range_v<T>>;
// ========================================================================== //



// ========================= IS BIDIRECTIONAL RANGE ========================= //
// Checks if a type is a bidirectional range
template <class T>
struct is_bidirectional_range
: is_range<T, std::bidirectional_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_bidirectional_range_v
= is_bidirectional_range<T>::value;

// Alias template
template <class T>
using if_bidirectional_range_t
= std::enable_if_t<is_bidirectional_range_v<T>>;
// ========================================================================== //



// ========================= IS RANDOM ACCESS RANGE ========================= //
// Checks if a type is a random access range
template <class T>
struct is_random_access_range
: is_range<T, std::random_access_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_random_access_range_v
= is_random_access_range<T>::value;

// Alias template
template <class T>
using if_random_access_range_t
= std::enable_if_t<is_random_access_range_v<T>>;
// ========================================================================== //



// ============================== IS CONTAINER ============================== //
// Checks if a type is a container: not a container
template <class T, class Category = void, class = void, class = void>
struct is_container: std::false_type {};

// Checks if a type is a container: it is a container
template <class Container, class Category>
struct is_container<Container, Category, std::enable_if_t<
        std::is_default_constructible_v<Container> &&
        std::is_copy_constructible_v<Container> &&
        std::is_move_constructible_v<Container> &&
        std::is_copy_assignable_v<Container> &&
        std::is_move_assignable_v<Container> &&
        std::is_destructible_v<Container> &&
        std::is_swappable_v<Container>
    >, std::void_t<
        typename Container::value_type,
        typename Container::reference,
        typename Container::const_reference,
        typename Container::iterator,
        typename Container::const_iterator,
        typename Container::difference_type,
        typename Container::size_type,
        decltype(std::declval<Container>().begin()),
        decltype(std::declval<Container>().end()),
        decltype(std::declval<Container>().cbegin()),
        decltype(std::declval<Container>().cend()),
        decltype(std::declval<Container>() == std::declval<Container>()),
        decltype(std::declval<Container>() != std::declval<Container>()),
        decltype(std::declval<Container>().size()),
        decltype(std::declval<Container>().max_size()),
        decltype(std::declval<Container>().empty())
    >
>: is_iterator<typename Container::iterator, Category> {};

// Variable template
template <class T, class Category = void>
inline constexpr bool is_container_v = is_container<T, Category>::value;

// Alias template
template <class T, class Category = void>
using if_container_t = std::enable_if_t<is_container_v<T, Category>>;
// ========================================================================== //



// ========================== IS DYNAMIC CONTAINER ========================== //
// Checks if a type is a dynamic container: not a dynamic container
template <class T, class Category = void, class = void>
struct is_dynamic_container: std::false_type {};

// Checks if a type is a dynamic container: it is a dynamic container
template <class Container, class Category>
struct is_dynamic_container<Container, Category, std::void_t<
    typename Container::allocator_type,
    decltype(std::declval<Container>().clear())
>>: is_container<Container, Category> {};

// Variable template
template <class T, class Category = void>
inline constexpr bool is_dynamic_container_v
= is_dynamic_container<T, Category>::value;

// Alias template
template <class T, class Category = void>
using if_dynamic_container_t
= std::enable_if_t<is_dynamic_container_v<T, Category>>;
// ========================================================================== //



// =========================== IS STATIC CONTAINER ========================== //
// Checks if a type is a static container
template <class T, class Category>
struct is_static_container: std::bool_constant<
    is_container_v<T, Category> && !is_dynamic_container_v<T, Category>
> {};

// Variable template
template <class T, class Category = void>
inline constexpr bool is_static_container_v
= is_static_container<T, Category>::value;

// Alias template
template <class T, class Category = void>
using if_static_container_t
= std::enable_if_t<is_static_container_v<T, Category>>;
// ========================================================================== //



// =========================== IS OUTPUT CONTAINER ========================== //
// Checks if a type is an output container
template <class T>
struct is_output_container
: is_container<T, std::output_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_output_container_v = is_output_container<T>::value;

// Alias template
template <class T>
using if_output_container_t = std::enable_if_t<is_output_container_v<T>>;
// ========================================================================== //



// =========================== IS INPUT CONTAINER =========================== //
// Checks if a type is an input container
template <class T>
struct is_input_container
: is_container<T, std::input_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_input_container_v = is_input_container<T>::value;

// Alias template
template <class T>
using if_input_container_t = std::enable_if_t<is_input_container_v<T>>;
// ========================================================================== //



// ========================== IS FORWARD CONTAINER ========================== //
// Checks if a type is a forward container
template <class T>
struct is_forward_container
: is_container<T, std::forward_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_forward_container_v = is_forward_container<T>::value;

// Alias template
template <class T>
using if_forward_container_t = std::enable_if_t<is_forward_container_v<T>>;
// ========================================================================== //



// ======================= IS BIDIRECTIONAL CONTAINER ======================= //
// Checks if a type is a bidirectional container
template <class T>
struct is_bidirectional_container
: is_container<T, std::bidirectional_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_bidirectional_container_v
= is_bidirectional_container<T>::value;

// Alias template
template <class T>
using if_bidirectional_container_t
= std::enable_if_t<is_bidirectional_container_v<T>>;
// ========================================================================== //



// ======================= IS RANDOM ACCESS CONTAINER ======================= //
// Checks if a type is a random access container
template <class T>
struct is_random_access_container
: is_container<T, std::random_access_iterator_tag> {};

// Variable template
template <class T>
inline constexpr bool is_random_access_container_v
= is_random_access_container<T>::value;

// Alias template
template <class T>
using if_random_access_container_t
= std::enable_if_t<is_random_access_container_v<T>>;
// ========================================================================== //



// ============================== IS ALLOCATOR ============================== //
// Checks if a type is an allocator: not an allocator
template <class T, class = void, class = void>
struct is_allocator: std::false_type {};

// Checks if a type is an allocator: it is an allocator
template <class Alloc>
struct is_allocator<Alloc, std::void_t<
        typename Alloc::value_type,
        decltype(std::declval<Alloc>().allocate(std::declval<std::size_t>()))
    >, std::void_t<typename std::allocator_traits<Alloc>::allocator_type>
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_allocator_v = is_allocator<T>::value;

// Alias template
template <class T>
using if_allocator_t = std::enable_if_t<is_allocator_v<T>>;
// ========================================================================== //



// ============================= IS EMPTY TUPLE ============================= //
// Checks if a type is an empty tuple: not an empty tuple
template <class T, class = void>
struct is_empty_tuple: std::false_type {};

// Checks if a type is an empty tuple: it is an empty tuple
template <class Tuple>
struct is_empty_tuple<Tuple, std::void_t<
    decltype(std::tuple_size<Tuple>::value)
>>: std::bool_constant<std::tuple_size_v<Tuple> == 0> {};

// Variable template
template <class T>
inline constexpr bool is_empty_tuple_v = is_empty_tuple<T>::value;

// Alias template
template <class T>
using if_empty_tuple_t = std::enable_if_t<is_empty_tuple_v<T>>;
// ========================================================================== //



// ================================ IS TUPLE ================================ //
// Checks if a type is a tuple: not a tuple
template <class T, class = void, class = void, class = void>
struct is_tuple: is_empty_tuple<T> {};

// Checks if a type is a tuple: it is a tuple
template <class Tuple>
struct is_tuple<Tuple, std::void_t<
        decltype(std::tuple_size<Tuple>::value)
    >, std::enable_if_t<(std::tuple_size_v<Tuple> > 0)>, std::void_t<
        std::tuple_element_t<0, Tuple>,
        decltype(std::get<0>(std::declval<Tuple>())),
        decltype(std::get<
            std::tuple_element_t<0, Tuple>
        >(std::declval<Tuple>()))
    >
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

// Alias template
template <class T>
using if_tuple_t = std::enable_if_t<is_tuple_v<T>>;
// ========================================================================== //



// ========================== IS SPECIALIZATION OF ========================== //
// Checks if a type is a specialization of a template: not a specialization
template <template <class...> class Template, class Specialization>
struct is_specialization_of: std::false_type {};

// Checks if a type is a specialization of a template: is a specialization
template <template <class...> class Template, class... Args>
struct is_specialization_of<Template, Template<Args...>>: std::true_type {};

// Variable template
template <template <class...> class Template, class Specialization>
inline constexpr bool is_specialization_of_v
= is_specialization_of<Template, Specialization>::value;

// Alias template
template <template <class...> class Template, class Specialization>
using if_specialization_of_t
= std::enable_if_t<is_specialization_of_v<Template, Specialization>>;
// ========================================================================== //



// ============================ COMMON TEMPLATE ============================= //
// Computes the common template of specializations: no common template
template <class... Specializations>
struct common_template;

// Computes the common template of specializations: recursion for types 
template <
    template <class...> class Template,
    class... Args0,
    class... Args1,
    class... Types
>
struct common_template<Template<Args0...>, Template<Args1...>, Types...> {
    template <class... Args>
    using temploid = typename common_template<
        Template<Args...>,
        Types...
    >::template temploid <Args...>;
};

// Computes the common template of specializations: recursion for nttps 
template <
    template <auto...> class Template,
    auto... Args0,
    auto... Args1,
    class... Types
>
struct common_template<Template<Args0...>, Template<Args1...>, Types...> {
    template <auto... Args>
    using temploid = typename common_template<
        Template<Args...>,
        Types...
    >::template temploid<Args...>;
};

// Computes the common template of specializations: recursion for mix 
template <
    template <class, auto...> class Template,
    class Arg0,
    auto... Args0,
    class Arg1,
    auto... Args1,
    class... Types
>
struct common_template<
    Template<Arg0, Args0...>,
    Template<Arg1, Args1...>,
    Types...
> {
    template <class Arg, auto... Args>
    using temploid = typename common_template<
        Template<Arg, Args...>,
        Types...
    >::template temploid<Arg, Args...>;
};

// Computes the common template of specializations: terminal for types
template <template <class...> class Template, class... Args0>
struct common_template<Template<Args0...>> {
    template <class... Args>
    using temploid = Template<Args...>;
};

// Computes the common template of specializations: terminal for nttps
template <template <auto...> class Template, auto... Args0>
struct common_template<Template<Args0...>> {
    template <auto... Args>
    using temploid = Template<Args...>;
};

// Computes the common template of specializations: terminal for mix
template <template <class, auto...> class Template, class Arg0, auto... Args0>
struct common_template<Template<Arg0, Args0...>> {
    template <class Arg, auto... Args>
    using temploid = Template<Arg, Args...>;
};

// Computes the common template of specializations: empty terminal
template <>
struct common_template<> {};
// ========================================================================== //



// ========================== HAVE COMMON TEMPLATE ========================== //
// Checks if types are specializations of a common template
template <class... Types>
struct have_common_template: std::is_destructible<common_template<Types...>> {};

// Variable template
template <class... Types>
inline constexpr bool have_common_template_v
= have_common_template<Types...>::value;

// Alias template
template <class... Types>
using if_common_template_t = std::enable_if_t<have_common_template_v<Types...>>;
// ========================================================================== //



// ============================ REBIND CONTAINER ============================ //
/*
template <class, class, class = void, class = void, class = void>
struct rebind_container;

template <template <class, auto...> class C, class To, class From, auto... X>
struct rebind_container<C<From, X...>, To, if_static_container_t<
        C<From, X...>
    >, if_static_container_t<C<To, X...>>, std::enable_if_t<
        std::is_same_v<typename C<From, X...>::value_type, From> &&
        std::is_same_v<typename C<To, X...>::value_type, To>
    >
> {
    using type = C<To, X...>;
};

template <template <class, class...> class C, class To, class From, class... X>
struct rebind_container<C<From, X...>, To, if_static_container_t<
        C<From, X...>
    >, if_static_container_t<C<To, X...>>, std::enable_if_t<
        std::is_same_v<typename C<From, X...>::value_type, From> &&
        std::is_same_v<typename C<To, X...>::value_type, To>
    >
> {
    using type = C<To, X...>;
};

template <template <class, auto...> class C, class To, class From, auto... X>
struct rebind_container<C<From, X...>, To, if_dynamic_container_t<
        C<From, X...>
    >, if_static_container_t<C<To, X...>>, std::enable_if_t<
        std::is_same_v<typename C<From, X...>::value_type, From> &&
        std::is_same_v<typename C<To, X...>::value_type, To>
    >
> {
    using type = C<To, X...>;
};

template <template <class, class...> class C, class To, class From, class... X>
struct rebind_container<C<From, X...>, To, if_dynamic_container_t<
        C<From, X...>
    >, if_static_container_t<C<To, X...>>, std::enable_if_t<
        std::is_same_v<typename C<From, X...>::value_type, From> &&
        std::is_same_v<typename C<To, X...>::value_type, To>
    >
> {
    using type = C<To, X...>;
};
*/
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _TRAITS_HPP_INCLUDED
// ========================================================================== //
