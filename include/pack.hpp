// ================================== PACK ================================== //
// Project:         epidesim
// Name:            pack.hpp
// Description:     Uniform management of parameter packs
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _PACK_HPP_INCLUDED
#define _PACK_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <utility>
#include <type_traits>
// Project sources
#include "wrappers.hpp"
#include "constants.hpp"
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// ================================= IS PACK ================================ //
// Checks if the type is a pack: not a pack
template <class T, class = void>
struct is_pack: std::false_type {};

// Checks if the type is a pack: it is a pack
template <class Pack>
struct is_pack<Pack, std::enable_if_t<Pack::is_pack::value>>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_pack_v = is_pack<T>::value;

// Alias template
template <class T>
using if_pack_t = std::enable_if_t<is_pack_v<T>>;
// ========================================================================== //



// ======================= PACKS FORWARD DECLARATIONS ======================= //
// Bool pack
template <bool... Bools>
struct bool_pack;

// Nttp pack
template <auto... Values>
struct nttp_pack;

// Type pack
template <class... Types>
struct type_pack;

// Nttp template pack
template <template <auto...> class... Templates>
struct nttp_template_pack;

// Type template pack
template <template <class...> class... Templates>
struct type_template_pack;

// Mixed template pack
template <template <class, auto...> class... Templates>
struct mixed_template_pack;
// ========================================================================== //



// ================================ PACK BASE =============================== //
// A generic base class for packs
template <class... Elements>
struct pack_base: Elements... {
    using is_pack = std::true_type;
    using is_pack_element = std::false_type;
    using index_sequence = std::index_sequence_for<Elements...>;
    using Elements::operator[]...;
    using Elements::get...;
    template <class Trait, class... Args>
    using apply_result = type_pack<
        decltype(Elements::template apply<Trait, Args...>())...
    >;
    template <class Trait, class... Args>
    using apply_result_t = type_pack<
        typename decltype(Elements::template apply<Trait, Args...>())::type...
    >;
    template <class Trait, class... Args>
    using apply_result_v = std::conditional_t<
        std::is_same_v<std::common_type_t<
            decltype(Elements::template apply<Trait, Args...>().value)...
        >, bool>,
        bool_pack<std::conditional_t<
            std::is_same_v<std::common_type_t<
                decltype(Elements::template apply<Trait, Args...>().value)...
            >, bool>,
            decltype(Elements::template apply<Trait, Args...>()),
            std::false_type
        >::value...>,
        nttp_pack<Elements::template apply<Trait, Args...>().value...>
    >;
    static constexpr std::size_t size() noexcept {
        return sizeof...(Elements);
    }
    template <class Trait, class... Args>
    static constexpr apply_result<Trait, Args...> apply() noexcept {
        return apply_result<Trait, Args...>{};
    }
    template <class Trait, class... Args>
    static constexpr apply_result_t<Trait, Args...> apply_t() noexcept {
        return apply_result_t<Trait, Args...>{};
    }
    template <class Trait, class... Args>
    static constexpr apply_result_v<Trait, Args...> apply_v() noexcept {
        return apply_result_v<Trait, Args...>{};
    }
};
// ========================================================================== //



// ============================= IS PACK ELEMENT ============================ //
// Checks if the type is a pack element: not a pack element
template <class T, class = void>
struct is_pack_element: std::false_type {};

// Checks if the type is a pack element: it is a pack element
template <class Element>
struct is_pack_element<
    Element,
    std::enable_if_t<Element::is_pack_element::value>
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_pack_element_v = is_pack_element<T>::value;

// Alias template
template <class T>
using if_pack_element_t = std::enable_if_t<is_pack_element_v<T>>;
// ========================================================================== //



// ============================ PACK ELEMENT BASE =========================== //
// A generic base class for pack elements
template <std::size_t Index, class Wrapper, class CRTP>
struct pack_element_base {
    using is_pack_element = std::true_type;
    using index_type = index_constant<Index>;
    using wrapper_type = Wrapper;
    using element_type = CRTP;
    static constexpr std::size_t index = index_type::value;
    using disambiguator = index_type;
    template <class... Args>
    static constexpr std::nullptr_t disambiguate = nullptr;
    using id_t = disambiguator;
    template <class Trait, class... Args>
    using apply_result = apply_trait<Trait, wrapper_type, Args...>;
    template <class Trait, class... Args>
    using apply_result_t = typename apply_result<Trait, Args...>::type;
    template <class Trait, class... Args>
    static constexpr auto apply_result_v = apply_result<Trait, Args...>::value;
    template <class Trait, class... Args>
    using apply_result_v_t = decltype(apply_result<Trait, Args...>::value);
    template <class T>
    using replace_result = std::conditional_t<T::index == index, T, CRTP>;
    template <bool Condition>
    using if_t = std::enable_if_t<Condition, std::nullptr_t>;
    template <std::size_t I>
    using if_same_index_t = if_t<I == index>;
    template <class W>
    using if_same_wrapper_t = if_t<std::is_same_v<W, wrapper_type>>;
    template <class Trait, class... Args>
    using if_applicable_t = if_t<std::is_void_v<
        if_apply_trait_t<Trait, wrapper_type, Args...>
    >>;
    constexpr wrapper_type& operator[](index_type) noexcept {
        return static_cast<wrapper_type&>(*this);
    }
    constexpr const wrapper_type& operator[](index_type) const noexcept {
        return static_cast<const wrapper_type&>(*this);
    }
    template <class W, disambiguator* = if_same_wrapper_t<W>{}>
    constexpr wrapper_type& operator[](const W&) noexcept {
        return static_cast<wrapper_type&>(*this);
    }
    template <class W, disambiguator* = if_same_wrapper_t<W>{}>
    constexpr const wrapper_type& operator[](const W&) const noexcept {
        return static_cast<const wrapper_type&>(*this);
    }
    template <class Trait, disambiguator** = if_applicable_t<Trait>{}>
    constexpr wrapper_type& operator[](const Trait&) noexcept {
        return static_cast<wrapper_type&>(*this);
    }
    template <class Trait, disambiguator** = if_applicable_t<Trait>{}>
    constexpr const wrapper_type& operator[](const Trait&) const noexcept {
        return static_cast<const wrapper_type&>(*this);
    }
    template <std::size_t I = index, disambiguator* = if_same_index_t<I>{}>
    constexpr element_type& get(index_type = {}) noexcept {
        return static_cast<element_type&>(*this);
    }
    template <std::size_t I = index, disambiguator* = if_same_index_t<I>{}>
    constexpr const element_type& get(index_type = {}) const noexcept {
        return static_cast<const element_type&>(*this);
    }
    template <class W, disambiguator** = if_same_wrapper_t<W>{}>
    constexpr element_type& get() noexcept {
        return static_cast<element_type&>(*this);
    }
    template <class W, disambiguator** = if_same_wrapper_t<W>{}>
    constexpr const element_type& get() const noexcept {
        return static_cast<const element_type&>(*this);
    }
    template <class Trait, class... X, id_t*** = if_applicable_t<Trait, X...>{}>
    constexpr element_type& get() noexcept {
        return static_cast<element_type&>(*this);
    }
    template <class Trait, class... X, id_t*** = if_applicable_t<Trait, X...>{}>
    constexpr const element_type& get() const noexcept {
        return static_cast<const element_type&>(*this);
    }
    template <class Trait, class... Args, disambiguator* = disambiguate<Trait>>
    static constexpr apply_result<Trait, Args...> apply() noexcept {
        return apply_result<Trait, Args...>{};
    }
    template <class Trait, class... Args, disambiguator* = disambiguate<Trait>>
    static constexpr apply_result_t<Trait, Args...> apply_t() noexcept {
        return apply_result_t<Trait, Args...>{};
    }
    template <class Trait, class... Args, disambiguator* = disambiguate<Trait>>
    static constexpr apply_result_v_t<Trait, Args...> apply_v() noexcept {
        return apply_result_v<Trait, Args...>;
    }
};
// ========================================================================== //



// ================================ BOOL PACK =============================== //
// An indexed element of a bool pack
template <std::size_t Index, bool Bool>
struct bool_pack_element
: pack_element_base<Index, bool_wrapper<Bool>, bool_pack_element<Index, Bool>> {
    using base = typename bool_pack_element::pack_element_base;
};

// The base class of a bool pack: declaration
template <class Indices, bool... Bools>
struct bool_pack_base;

// The base class of a bool pack: indexing specialization
template <std::size_t... Indices, bool... Bools>
struct bool_pack_base<std::index_sequence<Indices...>, Bools...>
: pack_base<bool_pack_element<Indices, Bools>...> {
    using base = typename bool_pack_base::pack_base;
    using common_type = std::common_type<decltype(Bools)...>;
    template <class Result = std::size_t>
    static constexpr Result sum() noexcept {
        return (static_cast<Result>(Bools) + ...);
    }
};

// A pack of bools
template <bool... Bools>
struct bool_pack: bool_pack_base<
    std::make_index_sequence<sizeof...(Bools)>,
    Bools...
> {
    using base = typename bool_pack::bool_pack_base;
};

// Makes a pack of bools
template <bool... Bools>
constexpr bool_pack<Bools...> pack() noexcept {
    return bool_pack<Bools...>{};
}
// ========================================================================== //



// ================================ NTTP PACK =============================== //
// An indexed element of a nttp pack
template <std::size_t Index, auto Value>
struct nttp_pack_element: pack_element_base<
    Index,
    nttp_wrapper<Value>,
    nttp_pack_element<Index, Value>
> {
    using base = typename nttp_pack_element::pack_element_base;
};

// The base class of a nttp pack: declaration
template <class Indices, auto... Values>
struct nttp_pack_base;

// The base class of a nttp pack: indexing specialization
template <std::size_t... Indices, auto... Values>
struct nttp_pack_base<std::index_sequence<Indices...>, Values...>
: pack_base<nttp_pack_element<Indices, Values>...> {
    using base = typename nttp_pack_base::pack_base;
    using common_type = std::common_type<decltype(Values)...>;
    template <class Result = std::size_t>
    static constexpr Result sum() noexcept {
        return (static_cast<Result>(Values) + ...);
    }
};

// A pack of nttps
template <auto... Values>
struct nttp_pack: nttp_pack_base<
    std::make_index_sequence<sizeof...(Values)>,
    Values...
> {
    using base = typename nttp_pack::nttp_pack_base;
};

// Makes a pack of nttps
template <auto... Values>
constexpr nttp_pack<Values...> pack() noexcept {
    return nttp_pack<Values...>{};
}
// ========================================================================== //



// ================================ TYPE PACK =============================== //
// An indexed element of a type pack
template <std::size_t Index, class Type>
struct type_pack_element
: pack_element_base<Index, type_wrapper<Type>, type_pack_element<Index, Type>> {
    using base = typename type_pack_element::pack_element_base;
};

// The base class of a type pack: declaration
template <class Indices, class... Types>
struct type_pack_base;

// The base class of a type pack: indexing specialization
template <std::size_t... Indices, class... Types>
struct type_pack_base<std::index_sequence<Indices...>, Types...>
: pack_base<type_pack_element<Indices, Types>...> {
    using base = typename type_pack_base::pack_base;
    using common_type = std::common_type<Types...>;
    template <class Result = std::size_t>
    static constexpr Result sum() noexcept {
        return (static_cast<Result>(Types{}) + ...);
    }
};

// A pack of types
template <class... Types>
struct type_pack: type_pack_base<std::index_sequence_for<Types...>, Types...> {
    using base = typename type_pack::type_pack_base;
};

// Makes a pack of types
template <class... Types>
constexpr type_pack<Types...> pack() noexcept {
    return type_pack<Types...>{};
}
// ========================================================================== //



// =========================== NTTP TEMPLATE PACK =========================== //
// An indexed element of a nttp template pack
template <std::size_t Index, template <auto...> class Template>
struct nttp_template_pack_element: pack_element_base<
    Index,
    nttp_template_wrapper<Template>,
    nttp_template_pack_element<Index, Template>
> {
    using base = typename nttp_template_pack_element::pack_element_base;
};

// The base class of a nttp template pack: declaration
template <class Indices, template <auto...> class... Templates>
struct nttp_template_pack_base;

// The base class of a nttp template pack: indexing specialization
template <std::size_t... Indices, template <auto...> class... Templates>
struct nttp_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<nttp_template_pack_element<Indices, Templates>...> {
    using base = typename nttp_template_pack_base::pack_base;
};

// A pack of nttp templates
template <template <auto...> class... Templates>
struct nttp_template_pack: nttp_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename nttp_template_pack::nttp_template_pack_base;
};

// Makes a pack of nttp templates
template <template <auto...> class... Templates>
constexpr nttp_template_pack<Templates...> pack() noexcept {
    return nttp_template_pack<Templates...>{};
}
// ========================================================================== //



// =========================== TYPE TEMPLATE PACK =========================== //
// An indexed element of a type template pack
template <std::size_t Index, template <class...> class Template>
struct type_template_pack_element: pack_element_base<
    Index,
    type_template_wrapper<Template>,
    type_template_pack_element<Index, Template>
> {
    using base = typename type_template_pack_element::pack_element_base;
};

// The base class of a type template pack: declaration
template <class Indices, template <class...> class... Templates>
struct type_template_pack_base;

// The base class of a type template pack: indexing specialization
template <std::size_t... Indices, template <class...> class... Templates>
struct type_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<type_template_pack_element<Indices, Templates>...> {
    using base = typename type_template_pack_base::pack_base;
};

// A pack of type templates
template <template <class...> class... Templates>
struct type_template_pack: type_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename type_template_pack::type_template_pack_base;
};

// Makes a pack of type templates
template <template <class...> class... Templates>
constexpr type_template_pack<Templates...> pack() noexcept {
    return type_template_pack<Templates...>{};
}
// ========================================================================== //



// =========================== MIXED TEMPLATE PACK ========================== //
// An indexed element of a mixed template pack
template <std::size_t Index, template <class, auto...> class Template>
struct mixed_template_pack_element: pack_element_base<
    Index,
    mixed_template_wrapper<Template>,
    mixed_template_pack_element<Index, Template>
> {
    using base = typename mixed_template_pack_element::pack_element_base;
};

// The base class of a mixed template pack: declaration
template <class Indices, template <class, auto...> class... Templates>
struct mixed_template_pack_base;

// The base class of a mixed template pack: indexing specialization
template <std::size_t... Indices, template <class, auto...> class... Templates>
struct mixed_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<mixed_template_pack_element<Indices, Templates>...> {
    using base = typename mixed_template_pack_base::pack_base;
};

// A pack of mixed templates
template <template <class, auto...> class... Templates>
struct mixed_template_pack: mixed_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename mixed_template_pack::mixed_template_pack_base;
};

// Makes a pack of mixed templates
template <template <class, auto...> class... Templates>
constexpr mixed_template_pack<Templates...> pack() noexcept {
    return mixed_template_pack<Templates...>{};
}
// ========================================================================== //



// =============================== MIXED PACK =============================== //
// The base class of a mixed pack: declaration
template <class Indices, class Type, auto... Values>
struct mixed_pack_base;

// The base class of a mixed pack: indexing specialization
template <std::size_t Index, std::size_t... Indices, class Type, auto... Values>
struct mixed_pack_base<std::index_sequence<Index, Indices...>, Type, Values...>
: pack_base<
    type_pack_element<Index, Type>,
    nttp_pack_element<Indices, Values>...
> {
    using base = typename mixed_pack_base::pack_base;
};

// A pack of mixed type and nttps
template <class Type, auto... Values>
struct mixed_pack: mixed_pack_base<
    std::make_index_sequence<sizeof...(Values) + 1>,
    Type,
    Values...
> {
    using base = typename mixed_pack::mixed_pack_base;
};

// Makes a pack of mixed type and ttps
template <class Type, auto Value, auto... Values>
constexpr mixed_pack<Type, Value, Values...> pack() noexcept {
    return mixed_pack<Type, Value, Values...>{};
}
// ========================================================================== //



// ================================ PACK SIZE =============================== //
// Returns the size of the pack: declaration
template <class T, class = void>
struct pack_size;

// Returns the size of the pack: pack specialization
template <class Pack>
struct pack_size<Pack, if_pack_t<Pack>>: size_constant<Pack::size()> {};

// Variable template
template <class Pack>
inline constexpr std::size_t pack_size_v = pack_size<Pack>::value;
// ========================================================================== //



// ============================== PACK ELEMENT ============================== //
// Returns the indexed element type of the pack: declaration
template <class T, std::size_t Index, class = void>
struct pack_element;

// Returns the indexed element type of the pack: pack specialization
template <class Pack, std::size_t Index>
struct pack_element<Pack, Index, if_pack_t<Pack>> {
    using type = std::remove_cv_t<std::remove_reference_t<
        decltype(std::declval<Pack>().template get<Index>())
    >>;
};

// Alias template
template <class Pack, std::size_t Index>
using pack_element_t = typename pack_element<Pack, Index>::type;
// ========================================================================== //



// =============================== PACK INDEX =============================== //
// Returns the index of a pack element: declaration
template <class T, class = void>
struct pack_index;

// Returns the index of a pack element: pack element specialization
template <class Element>
struct pack_index<Element, if_pack_element_t<Element>>
: index_constant<Element::index> {};

// Variable template
template <class Element>
inline constexpr std::size_t pack_index_v = pack_index<Element>::value;
// ========================================================================== //



// ================================ PACK GET ================================ //
// Returns a reference to the element specified by the provided index argument
template <class Pack, std::size_t Index>
constexpr decltype(std::declval<Pack>().template get<Index>()) pack_get(
    Pack&& pack,
    index_constant<Index> index
) noexcept {
    return std::forward<Pack>(pack).get(index);
}

// Returns a reference to the element specified by the provided template index
template <std::size_t Index, class Pack>
constexpr decltype(std::declval<Pack>().template get<Index>()) pack_get(
    Pack&& pack
) noexcept {
    return std::forward<Pack>(pack).template get<Index>();
}

// Returns a reference to the element specified by the provided templates
template <class... Args, class Pack>
constexpr decltype(std::declval<Pack>().template get<Args...>()) pack_get(
    Pack&& pack
) noexcept {
    return std::forward<Pack>(pack).template get<Args...>();
}
// ========================================================================== //



// ================================ PACK FROM =============================== //
// Makes a pack from template arguments: declaration
template <class Template>
struct pack_from;

// Makes a pack from template arguments: nttp arguments
template <template <auto...> class Template, auto... Values>
struct pack_from<Template<Values...>> {
    using type = nttp_pack<Values...>;
};

// Makes a pack from template arguments: type arguments
template <template <class...> class Template, class... Types>
struct pack_from<Template<Types...>> {
    using type = type_pack<Types...>;
};

// Makes a pack from template arguments: mixed arguments
template <template <class, auto...> class Template, class Type, auto... Values>
struct pack_from<Template<Type, Values...>> {
    using type = mixed_pack<Type, Values...>;
};

// Alias template
template <class Template>
using pack_from_t = typename pack_from<Template>::type;
// ========================================================================== //



// ================================= PACK TO ================================ //
// Makes a template from pack arguments: declaration
template <class Pack>
struct pack_to;

// Makes a template from pack arguments: nttp arguments
template <auto... Values>
struct pack_to<nttp_pack<Values...>> {
    template <template <auto...> class Template>
    using temploid = Template<Values...>;
};

// Makes a template from pack arguments: type arguments
template <class... Types>
struct pack_to<type_pack<Types...>> {
    template <template <class...> class Template>
    using temploid = Template<Types...>;
};

// Makes a template from pack arguments: mixed arguments
template <class Type, auto... Values>
struct pack_to<mixed_pack<Type, Values...>> {
    template <template <class, auto> class Template>
    using temploid = Template<Type, Values...>;
};
// ========================================================================== //



// ========================= SWAP TEMPLATE ARGUMENTS ======================== //
// Swaps template arguments between two templates: declaration
template <class Lhs, class Rhs>
struct swap_template_arguments;

// Swaps template arguments between two templates: nttp arguments
template <
    template <auto...> class Lhs,
    auto... Ls,
    template <auto...> class Rhs,
    auto... Rs
>
struct swap_template_arguments<Lhs<Ls...>, Rhs<Rs...>> {
    using lhs = Lhs<Rs...>;
    using rhs = Rhs<Ls...>;
};

// Swaps template arguments between two templates: type arguments
template <
    template <class...> class Lhs,
    class... Ls,
    template <class...> class Rhs,
    class... Rs
>
struct swap_template_arguments<Lhs<Ls...>, Rhs<Rs...>> {
    using lhs = Lhs<Rs...>;
    using rhs = Rhs<Ls...>;
};

// Swaps template arguments between two templates: mixed arguments
template <
    template <class, auto...> class Lhs,
    class L,
    auto... Ls,
    template <class, auto...> class Rhs,
    class R,
    auto... Rs
>
struct swap_template_arguments<Lhs<L, Ls...>, Rhs<R, Rs...>> {
    using lhs = Lhs<R, Rs...>;
    using rhs = Rhs<L, Ls...>;
};
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _PACK_HPP_INCLUDED
// ========================================================================== //
