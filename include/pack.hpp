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
    using pack_result = type_pack<
        decltype(Elements::template apply<Trait, Args...>())...
    >;
    template <class Trait, class... Args>
    using pack_result_t = type_pack<
        typename decltype(Elements::template apply<Trait, Args...>())::type...
    >;
    template <class Trait, class... Args>
    using pack_result_v = std::conditional_t<
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
    static constexpr pack_result<Trait, Args...> apply() noexcept {
        return pack_result<Trait, Args...>{};
    }
    template <class Trait, class... Args>
    static constexpr pack_result_t<Trait, Args...> apply_t() noexcept {
        return pack_result_t<Trait, Args...>{};
    }
    template <class Trait, class... Args>
    static constexpr pack_result_v<Trait, Args...> apply_v() noexcept {
        return pack_result_v<Trait, Args...>{};
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


/*
// ================================ NTTP PACK =============================== //
// A wrapper for an element of a nttp pack: declaration
template <auto Value, std::size_t... Index>
struct nttp_pack_element;

// A wrapper for an element of a nttp pack: non-indexed version
template <auto Value>
struct nttp_pack_element<Value> {
    using type = nttp_pack_element<Value>;
    using value_type = decltype(Value);
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
    static constexpr value_type value = Value;
};

// A wrapper for an element of a nttp pack: indexed version
template <auto Value, std::size_t Index>
struct nttp_pack_element<Value, Index>
: pack_element_base<
    Index,
    nttp_pack_element<Value>,
    nttp_pack_element<Value, Index>
> {
    using base = typename nttp_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <auto V, class = std::enable_if_t<V == Value>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <auto, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Value, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <template <auto, class...> class Trait, class... Args>
    static constexpr Trait<Value, Args...> apply(element_index = {}) noexcept {
        return Trait<Value, Args...>{};
    }
};

// The base class of a nttp pack: declaration
template <class Indices, auto... Values>
struct nttp_pack_base;

// The base class of a nttp pack: indexing specialization
template <std::size_t... Indices, auto... Values>
struct nttp_pack_base<std::index_sequence<Indices...>, Values...>
: pack_base<sizeof...(Indices)>, nttp_pack_element<Values, Indices>... {
    using nttp_pack_element<Values, Indices>::operator[]...;
    using nttp_pack_element<Values, Indices>::at...;
    using nttp_pack_element<Values, Indices>::get...;
    using nttp_pack_element<Values, Indices>::extract...;
    using nttp_pack_element<Values, Indices>::match...;
};

// A pack of nttps
template <auto... Values>
struct nttp_pack: nttp_pack_base<
    std::make_index_sequence<sizeof...(Values)>,
    Values...
> {
    using base = typename nttp_pack::nttp_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of nttps
template <auto... Values>
constexpr nttp_pack<Values...> make_pack() noexcept {
    return nttp_pack<Values...>{};
}
// ========================================================================== //




// =========================== TYPE TEMPLATE PACK =========================== //
// A wrapper for an element of a type template pack: declaration
template <template <class...> class Template, std::size_t... Index>
struct type_template_pack_element;

// A wrapper for an element of a type template pack: non-indexed version
template <template <class...> class Template>
struct type_template_pack_element<Template> {
    template <class... Args>
    using temploid = Template<Args...>;
};

// A wrapper for an element of a type template pack: indexed version
template <template <class...> class Template, std::size_t Index>
struct type_template_pack_element<Template, Index>
: pack_element_base<
    Index,
    type_template_pack_element<Template>,
    type_template_pack_element<Template, Index>
> {
    using base = typename type_template_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <template <class...> class T, class Dummy = void>
    struct is_same_template: std::false_type {};
    template <class Dummy>
    struct is_same_template<Template, Dummy>: std::true_type {};
    template <template <class...> class T>
    static constexpr bool is_same_template_v = is_same_template<T>::value;
    template <template <class...> class T>
    using if_same_template_t = std::enable_if_t<is_same_template_v<T>>;
    template <template <class...> class T, class = if_same_template_t<T>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <template <template <class...> class, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Template, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <template <class...> class, class...> class Trait,
        class... Args,
        class Result = Trait<Template, Args...>
    >
    static constexpr Result apply(element_index = {}) noexcept {
        return Trait<Value, Args...>{};
    }
};

// The base class of a type template pack: declaration
template <class Indices, template <class...> class... Templates>
struct type_template_pack_base;

// The base class of a type template pack: indexing specialization
template <std::size_t... Indices, template <class...> class... Templates>
struct type_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<sizeof...(Indices)>
, type_template_pack_element<Templates, Indices>... {
    using type_template_pack_element<Templates, Indices>::operator[]...;
    using type_template_pack_element<Templates, Indices>::at...;
    using type_template_pack_element<Templates, Indices>::get...;
    using type_template_pack_element<Templates, Indices>::extract...;
    using type_template_pack_element<Templates, Indices>::match...;
};

// A pack of type templates
template <template <class...> class... Templates>
struct type_template_pack: type_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename type_template_pack::type_template_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of type templates
template <template <class...> class... Templates>
constexpr type_template_pack<Templates...> make_pack() noexcept {
    return type_template_pack<Templates...>{};
}
// ========================================================================== //



// =========================== NTTP TEMPLATE PACK =========================== //
// A wrapper for an element of a nttp template pack: declaration
template <template <auto...> class Template, std::size_t... Index>
struct nttp_template_pack_element;

// A wrapper for an element of a nttp template pack: non-indexed version
template <template <auto...> class Template>
struct nttp_template_pack_element<Template> {
    template <auto... Args>
    using temploid = Template<Args...>;
};

// A wrapper for an element of a nttp template pack: indexed version
template <template <auto...> class Template, std::size_t Index>
struct nttp_template_pack_element<Template, Index>
: pack_element_base<
    Index,
    nttp_template_pack_element<Template>,
    nttp_template_pack_element<Template, Index>
> {
    using base = typename nttp_template_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <template <auto...> class T, class Dummy = void>
    struct is_same_template: std::false_type {};
    template <class Dummy>
    struct is_same_template<Template, Dummy>: std::true_type {};
    template <template <auto...> class T>
    static constexpr bool is_same_template_v = is_same_template<T>::value;
    template <template <auto...> class T>
    using if_same_template_t = std::enable_if_t<is_same_template_v<T>>;
    template <template <auto...> class T, class = if_same_template_t<T>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <template <auto...> class, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Template, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <template <auto...> class, class...> class Trait,
        class... Args,
        class Result = Trait<Template, Args...>
    >
    static constexpr Result apply(element_index = {}) noexcept {
        return Trait<Value, Args...>{};
    }
};

// The base class of a nttp template pack: declaration
template <class Indices, template <auto...> class... Templates>
struct nttp_template_pack_base;

// The base class of a nttp template pack: indexing specialization
template <std::size_t... Indices, template <auto...> class... Templates>
struct nttp_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<sizeof...(Indices)>
, nttp_template_pack_element<Templates, Indices>... {
    using nttp_template_pack_element<Templates, Indices>::operator[]...;
    using nttp_template_pack_element<Templates, Indices>::at...;
    using nttp_template_pack_element<Templates, Indices>::get...;
    using nttp_template_pack_element<Templates, Indices>::extract...;
    using nttp_template_pack_element<Templates, Indices>::match...;
};

// A pack of nttp templates
template <template <auto...> class... Templates>
struct nttp_template_pack: nttp_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename nttp_template_pack::nttp_template_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of nttp templates
template <template <auto...> class... Templates>
constexpr nttp_template_pack<Templates...> make_pack() noexcept {
    return nttp_template_pack<Templates...>{};
}
// ========================================================================== //



// =========================== MIXED TEMPLATE PACK ========================== //
// A wrapper for an element of a mixed template pack: declaration
template <template <class, auto...> class Template, std::size_t... Index>
struct mixed_template_pack_element;

// A wrapper for an element of a mixed template pack: non-indexed version
template <template <class, auto...> class Template>
struct mixed_template_pack_element<Template> {
    template <class Arg, auto... Args>
    using temploid = Template<Arg, Args...>;
};

// A wrapper for an element of a mixed template pack: indexed version
template <template <class, auto...> class Template, std::size_t Index>
struct mixed_template_pack_element<Template, Index>
: pack_element_base<
    Index,
    mixed_template_pack_element<Template>,
    mixed_template_pack_element<Template, Index>
> {
    using base = typename mixed_template_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <template <class, auto...> class T, class Dummy = void>
    struct is_same_template: std::false_type {};
    template <class Dummy>
    struct is_same_template<Template, Dummy>: std::true_type {};
    template <template <class, auto...> class T>
    static constexpr bool is_same_template_v = is_same_template<T>::value;
    template <template <class, auto...> class T>
    using if_same_template_t = std::enable_if_t<is_same_template_v<T>>;
    template <template <class, auto...> class T, class = if_same_template_t<T>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <template <class, auto...> class, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Template, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <template <class, auto...> class, class...> class Trait,
        class... Args,
        class Result = Trait<Template, Args...>
    >
    static constexpr Result apply(element_index = {}) noexcept {
        return Trait<Value, Args...>{};
    }
};

// The base class of a mixed template pack: declaration
template <class Indices, template <class, auto...> class... Templates>
struct mixed_template_pack_base;

// The base class of a mixed template pack: indexing specialization
template <std::size_t... Indices, template <class, auto...> class... Templates>
struct mixed_template_pack_base<std::index_sequence<Indices...>, Templates...>
: pack_base<sizeof...(Indices)>
, mixed_template_pack_element<Templates, Indices>... {
    using mixed_template_pack_element<Templates, Indices>::operator[]...;
    using mixed_template_pack_element<Templates, Indices>::at...;
    using mixed_template_pack_element<Templates, Indices>::get...;
    using mixed_template_pack_element<Templates, Indices>::extract...;
    using mixed_template_pack_element<Templates, Indices>::match...;
};

// A pack of mixed templates
template <template <class, auto...> class... Templates>
struct mixed_template_pack: mixed_template_pack_base<
    std::make_index_sequence<sizeof...(Templates)>,
    Templates...
> {
    using base = typename mixed_template_pack::mixed_template_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of mixed templates
template <template <class, auto...> class... Templates>
constexpr mixed_template_pack<Templates...> make_pack() noexcept {
    return mixed_template_pack<Templates...>{};
}
// ========================================================================== //



// ================================= IS PACK ================================ //
// Checks if a type is a pack: not a pack
template <class T, class = void>
struct is_pack: std::false_type {};

// Checks if a type is a pack: it is a pack
template <class Pack>
struct is_pack<Pack, std::enable_if_t<Pack::is_pack::value>>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_pack_v = is_pack<T>::value;

// Alias template
template <class T>
using if_pack_t = std::enable_if_t<is_pack_v<T>>;
// ========================================================================== //



// ============================== PACK ELEMENT ============================== //
// Returns the wrapped element type of the pack: declaration
template <class T, std::size_t Index, class = void>
struct pack_element;

// Returns the wrapped element type of the pack: pack specialization
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



// ============================== PACK EXTRACT ============================== //
// Returns the indexed element type of the pack: declaration
template <class T, std::size_t Index, class = void>
struct pack_extract;

// Returns the indexed element type of the pack: pack specialization
template <class Pack, std::size_t Index>
struct pack_extract<Pack, Index, if_pack_t<Pack>> {
    using type = decltype(Pack::template extract<Index>());
};

// Alias template
template <class Pack, std::size_t Index>
using pack_extract_t = typename pack_extract<Pack, Index>::type;
// ========================================================================== //
*/


// ========================================================================== //
} // namespace epidesim
#endif // _PACK_HPP_INCLUDED
// ========================================================================== //
