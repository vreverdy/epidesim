// ================================== PACK ================================== //
// Project:         epidesim
// Name:            pack.hpp
// Description:     Management of parameter packs
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
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// =========================== PACK ELEMENT INDEX =========================== //
// The type of a pack element index
template <std::size_t Index>
struct pack_element_index: std::integral_constant<std::size_t, Index> {
    using integral_constant = std::integral_constant<std::size_t, Index>;
    using std::integral_constant<std::size_t, Index>::integral_constant;
    constexpr pack_element_index() noexcept = default;
    constexpr pack_element_index(integral_constant) noexcept {
    }
};

// Variable template
template <std::size_t Index>
inline constexpr pack_element_index<Index> pack_index = {};
// ========================================================================== //



// ================================ PACK BASE =============================== //
// A generic base class for pack elements using CRTP
template <std::size_t Index, class Base, class Derived>
struct pack_element_base: private Base {
    using element = Derived;
    using element_type = Base;
    using element_index = pack_element_index<Index>;
    static constexpr element_index pack_index = {}; 
    static constexpr std::size_t index = pack_index;
    constexpr element& operator[](element_index) noexcept {
        return static_cast<element&>(*this);
    }
    constexpr const element& operator[](element_index) const noexcept {
        return static_cast<element&>(*this);
    }
    template <std::size_t I = index, class = std::enable_if_t<I == index>>
    constexpr element& at(element_index = {}) noexcept {
        return static_cast<element&>(*this);
    }
    template <std::size_t I = index, class = std::enable_if_t<I == index>>
    constexpr const element& at(element_index = {}) const noexcept {
        return static_cast<element&>(*this);
    }
    template <std::size_t I = index, class = std::enable_if_t<I == index>>
    constexpr element_type& get(element_index = {}) noexcept {
        return static_cast<element_type&>(*this);
    }
    template <std::size_t I = index, class = std::enable_if_t<I == index>>
    constexpr const element_type& get(element_index = {}) const noexcept {
        return static_cast<element_type&>(*this);
    }
    template <std::size_t I = index, class = std::enable_if_t<I == index>>
    static constexpr element extract(element_index = {}) noexcept {
        return element{};
    }
};

// A generic base class for packs
template <std::size_t Size>
struct pack_base {
    using is_pack = std::true_type;
    using index_sequence = std::make_index_sequence<Size>;
    static constexpr std::size_t size() noexcept {
        return Size;
    }
};
// ========================================================================== //


/*
// =============================== BOOL TRAIT =============================== //
// A trait wrapper for a boolean pack
template <template <bool, class...> Trait>
struct bool_pack_trait {
    template <bool Bool, class... Args>
    using temploid = Trait<Bool, Args...>;
    template <bool Bool, class... Args>
    using type = typename Trait<Bool, Args...>::type;
    template <bool Bool, class... Args>
    static constexpr auto value = Trait<Bool, Args...>::value;
};

// Type alias template
template <template <bool, class...> Trait, bool Bool, class... Args>
using bool_pack_trait_t
= typename bool_pack_trait<Trait>::template temploid<Bool, Args...>::type;

// Value variable template
template <template <bool, class...> Trait, bool Bool, class... Args>
inline constexpr auto bool_pack_trait_v
= bool_pack_trait<Trait>::template temploid<Bool, Args...>::value;

// Enable if alias template
template <template <bool, class...> Trait, bool Bool, class... Args>
using if_bool_pack_trait_t
= std::enable_if_t<bool_pack_trait_v<Trait<Bool, Args...>>>;
// ========================================================================== //
*/


// ================================ BOOL PACK =============================== //
// A wrapper for an element of a boolean pack: declaration
template <bool Bool, std::size_t... Index>
struct bool_pack_element;

// A wrapper for an element of a boolean pack: non-indexed version
template <bool Bool>
struct bool_pack_element<Bool>: std::bool_constant<Bool> {};

// A wrapper for an element of a boolean pack: indexed version
template <bool Bool, std::size_t Index>
struct bool_pack_element<Bool, Index>
: pack_element_base<
    Index,
    bool_pack_element<Bool>,
    bool_pack_element<Bool, Index>
> {
    using base = typename bool_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <bool B, class = std::enable_if_t<B == Bool>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <bool, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Bool, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <template <bool, class...> class Trait, class... Args>
    static constexpr Trait<Bool, Args...> apply(element_index = {}) noexcept {
        return Trait<Bool, Args...>{};
    }
};

// The base class of a boolean pack: declaration
template <class Indices, bool... Bools>
struct bool_pack_base;

// The base class of a boolean pack: indexing specialization
template <std::size_t... Indices, bool... Bools>
struct bool_pack_base<std::index_sequence<Indices...>, Bools...>
: pack_base<sizeof...(Indices)>, bool_pack_element<Bools, Indices>... {
    using bool_pack_element<Bools, Indices>::operator[]...;
    using bool_pack_element<Bools, Indices>::at...;
    using bool_pack_element<Bools, Indices>::get...;
    using bool_pack_element<Bools, Indices>::extract...;
    using bool_pack_element<Bools, Indices>::match...;
};

// A pack of boolean values
template <bool... Bools>
struct bool_pack: bool_pack_base<
    std::make_index_sequence<sizeof...(Bools)>,
    Bools...
> {
    using base = typename bool_pack::bool_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of boolean values
template <bool... Bools>
constexpr bool_pack<Bools...> make_pack() noexcept {
    return bool_pack<Bools...>{};
}
// ========================================================================== //



// ================================ TYPE PACK =============================== //
// A trait wrapper for a type pack
template <template <class, class...> Trait>
struct type_pack_trait {
    template <class Type, class... Args>
    using temploid = Trait<Type, Args...>;
};

// A trait wrapper for a type pack: alias template
template <template <bool, class...> Trait, bool Bool, class... Args>
using type_pack_trait_t
= typename type_pack_trait<Trait>::template temploid<Bool, Args...>::type;

// A trait wrapper for a type pack: variable template
template <template <bool, class...> Trait, bool Bool, class... Args>
inline constexpr auto type_pack_trait_v
= type_pack_trait<Trait>::template temploid<Bool, Args...>::value;

// A wrapper for an element of a type pack: declaration
template <class Type, std::size_t... Index>
struct type_pack_element;

// A wrapper for an element of a type pack: non-indexed version
template <class Type>
struct type_pack_element<Type> {
    using type = Type;
};

// A wrapper for an element of a type pack: indexed version
template <class Type, std::size_t Index>
struct type_pack_element<Type, Index>
: pack_element_base<
    Index,
    type_pack_element<Type>,
    type_pack_element<Type, Index>
> {
    using base = typename type_pack_element::pack_element_base;
    using element = typename base::element;
    using element_type = typename base::element_type;
    using element_index = typename base::element_index;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    template <class T, class = std::enable_if_t<std::is_same_v<T, Type>>>
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <
        template <class, class...> class Trait,
        class... Args,
        class = std::enable_if_t<Trait<Type, Args...>::value>
    >
    static constexpr element match(element_index = {}) noexcept {
        return element{};
    }
    template <template <class, class...> class Trait, class... Args>
    static constexpr Trait<Type, Args...> apply(element_index = {}) noexcept {
        return Trait<Type, Args...>{};
    }
};

// The base class of a type pack: declaration
template <class Indices, class... Types>
struct type_pack_base;

// The base class of a type pack: indexing specialization
template <std::size_t... Indices, class... Types>
struct type_pack_base<std::index_sequence<Indices...>, Types...>
: pack_base<sizeof...(Indices)>, type_pack_element<Types, Indices>... {
    using type_pack_element<Types, Indices>::operator[]...;
    using type_pack_element<Types, Indices>::at...;
    using type_pack_element<Types, Indices>::get...;
    using type_pack_element<Types, Indices>::extract...;
    using type_pack_element<Types, Indices>::match...;
};

// A pack of types
template <class... Types>
struct type_pack: type_pack_base<
    std::make_index_sequence<sizeof...(Types)>,
    Types...
> {
    using base = typename type_pack::type_pack_base;
    using base::operator[];
    using base::at;
    using base::get;
    using base::extract;
    using base::match;
};

// Makes a pack of types
template <class... Types>
constexpr type_pack<Types...> make_pack() noexcept {
    return type_pack<Types...>{};
}
// ========================================================================== //



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



// ========================================================================== //
} // namespace epidesim
#endif // _PACK_HPP_INCLUDED
// ========================================================================== //
