// ================================ WRAPPERS ================================ //
// Project:         epidesim
// Name:            wrappers.hpp
// Description:     Wrappers of different kinds to provide uniform interfaces
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2020-]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _WRAPPERS_HPP_INCLUDED
#define _WRAPPERS_HPP_INCLUDED
// ========================================================================== //



// ============================== PREAMBLE ================================== //
// C++ standard library
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace epidesim {
// ========================================================================== //



// =============================== IS WRAPPER =============================== //
// Checks if the type is a wrapper: not a wrapper
template <class T, class = void>
struct is_wrapper: std::false_type {};

// Checks if the type is a wrapper: it is a wrapper
template <class Wrapper>
struct is_wrapper<
    Wrapper,
    std::enable_if_t<Wrapper::is_wrapper::value>
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_wrapper_v = is_wrapper<T>::value;

// Alias template
template <class T>
using if_wrapper_t = std::enable_if_t<is_wrapper_v<T>>;
// ========================================================================== //



// ============================== WRAPPER BASE ============================== //
// The common base of wrappers
struct wrapper_base {
    using is_wrapper = std::true_type;
};
// ========================================================================== //



// ============================== BOOL WRAPPER ============================== //
// A wrapper to hold a bool
template <bool Bool>
struct bool_wrapper: wrapper_base {
    using wrapper_type = bool_wrapper<Bool>;
    template <bool Other>
    using rebind_wrapper = bool_wrapper<Other>;
    using value_type = bool;
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
    static constexpr value_type value = Bool;
};

// Makes a wrapper that holds a bool
template <bool Bool>
constexpr bool_wrapper<Bool> wrap() noexcept {
    return bool_wrapper<Bool>{};
}
// ========================================================================== //



// ============================== NTTP WRAPPER ============================== //
// A wrapper to hold a nttp
template <auto Value>
struct nttp_wrapper: wrapper_base {
    using wrapper_type = nttp_wrapper<Value>;
    template <auto Other>
    using rebind_wrapper = nttp_wrapper<Other>;
    using value_type = decltype(Value);
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
    static constexpr value_type value = Value;
};

// Makes a wrapper that holds a nttp
template <auto Value>
constexpr nttp_wrapper<Value> wrap() noexcept {
    return nttp_wrapper<Value>{};
}
// ========================================================================== //



// ============================== TYPE WRAPPER ============================== //
// A wrapper to hold a type
template <class Type>
struct type_wrapper: wrapper_base {
    using wrapper_type = type_wrapper<Type>;
    template <class Other>
    using rebind_wrapper = type_wrapper<Other>;
    using type = Type;
};

// Makes a wrapper that holds a type
template <class Type>
constexpr type_wrapper<Type> wrap() noexcept {
    return type_wrapper<Type>{};
}
// ========================================================================== //



// ========================== NTTP TEMPLATE WRAPPER ========================= //
// A wrapper to hold a nttp template
template <template <auto...> class Template>
struct nttp_template_wrapper: wrapper_base {
    using wrapper_type = nttp_template_wrapper<Template>;
    template <template <auto...> class Other>
    using rebind_wrapper = nttp_template_wrapper<Other>;
    template <auto... Args>
    using temploid = Template<Args...>;
};

// Makes a wrapper that holds a nttp template
template <template <auto...> class Template>
constexpr nttp_template_wrapper<Template> wrap() noexcept {
    return nttp_template_wrapper<Template>{};
}
// ========================================================================== //



// ========================== TYPE TEMPLATE WRAPPER ========================= //
// A wrapper to hold a type template
template <template <class...> class Template>
struct type_template_wrapper: wrapper_base {
    using wrapper_type = type_template_wrapper<Template>;
    template <template <class...> class Other>
    using rebind_wrapper = type_template_wrapper<Other>;
    template <class... Args>
    using temploid = Template<Args...>;
};

// Makes a wrapper that holds a type template
template <template <class...> class Template>
constexpr type_template_wrapper<Template> wrap() noexcept {
    return type_template_wrapper<Template>{};
}
// ========================================================================== //



// ========================= MIXED TEMPLATE WRAPPER ========================= //
// A wrapper to hold a mixed template
template <template <class, auto...> class Template>
struct mixed_template_wrapper: wrapper_base {
    using wrapper_type = mixed_template_wrapper<Template>;
    template <template <class, auto...> class Other>
    using rebind_wrapper = mixed_template_wrapper<Other>;
    template <class Arg, auto... Args>
    using temploid = Template<Arg, Args...>;
};

// Makes a wrapper that holds a mixed template
template <template <class, auto...> class Template>
constexpr mixed_template_wrapper<Template> wrap() noexcept {
    return mixed_template_wrapper<Template>{};
}
// ========================================================================== //



// ============================ IS TRAIT WRAPPER ============================ //
// Checks if the type is a trait wrapper: not a trait wrapper
template <class T, class = void>
struct is_trait_wrapper: std::false_type {};

// Checks if the type is a trait wrapper: it is a trait wrapper
template <class Wrapper>
struct is_trait_wrapper<
    Wrapper,
    std::enable_if_t<Wrapper::is_trait_wrapper::value>
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_trait_wrapper_v = is_trait_wrapper<T>::value;

// Alias template
template <class T>
using if_trait_wrapper_t = std::enable_if_t<is_trait_wrapper_v<T>>;
// ========================================================================== //



// =========================== TRAIT WRAPPER BASE =========================== //
// The common base of trait wrappers
struct trait_wrapper_base: wrapper_base {
    using is_trait_wrapper = std::true_type;
};
// ========================================================================== //



// =============================== APPLY TRAIT ============================== //
// Applies a wrapped trait on a wrapped entity
template <class Trait, class Entity, class... Args>
struct apply_trait;

// Alias template to type
template <class Trait, class Entity, class... Args>
using apply_trait_t
= typename apply_trait<Trait, Entity, Args...>::type;

// Variable template to value
template <class Trait, class Entity, class... Args>
inline constexpr auto apply_trait_v
= apply_trait<Trait, Entity, Args...>::value;

// Alias template to enable if
template <class Trait, class Entity, class... Args>
using if_apply_trait_t
= std::enable_if_t<apply_trait_v<Trait, Entity, Args...>>;
// ========================================================================== //



// =============================== NTTP TRAIT =============================== //
// A wrapper to hold a trait on a bool
template <template <bool, class...> class Trait>
struct bool_trait: trait_wrapper_base {
    using wrapper_type = bool_trait<Trait>;
    template <template <bool, class...> class Other>
    using rebind_wrapper = bool_trait<Other>;
    template <bool Bool, class... Args>
    using temploid = Trait<Bool, Args...>;
    template <bool Bool, class... Args>
    using type = typename Trait<Bool, Args...>::type;
    template <bool Bool, class... Args>
    static constexpr auto value = Trait<Bool, Args...>::value;
};

// Alias template to type
template <template <bool, class...> class Trait, bool Bool, class... Args>
using bool_trait_t
= typename bool_trait<Trait>::template type<Bool, Args...>;

// Variable template to value
template <template <bool, class...> class Trait, bool Bool, class... Args>
inline constexpr auto bool_trait_v
= bool_trait<Trait>::template type<Bool, Args...>::value;

// Alias template to enable if
template <template <bool, class...> class Trait, bool Bool, class... Args>
using if_bool_trait_t
= std::enable_if_t<bool_trait_v<Trait, Bool, Args...>>;

// Makes a wrapper that holds a bool trait
template <template <bool, class...> class Trait>
constexpr bool_trait<Trait> wrap_trait() noexcept {
    return bool_trait<Trait>{};
}

// Applies a wrapped bool trait on a wrapped bool
template <template <bool, class...> class Trait, bool Value, class... Args>
struct apply_trait<bool_trait<Trait>, bool_wrapper<Value>, Args...> {
    using type = typename Trait<Value, Args...>::type;
    static constexpr bool value = Trait<Value, Args...>::value;
};
// ========================================================================== //



// =============================== NTTP TRAIT =============================== //
// A wrapper to hold a trait on a nttp
template <template <auto, class...> class Trait>
struct nttp_trait: trait_wrapper_base {
    using wrapper_type = nttp_trait<Trait>;
    template <template <auto, class...> class Other>
    using rebind_wrapper = nttp_trait<Other>;
    template <auto Value, class... Args>
    using temploid = Trait<Value, Args...>;
    template <auto Value, class... Args>
    using type = typename Trait<Value, Args...>::type;
    template <auto Value, class... Args>
    static constexpr auto value = Trait<Value, Args...>::value;
};

// Alias template to type
template <template <auto, class...> class Trait, auto Value, class... Args>
using nttp_trait_t
= typename nttp_trait<Trait>::template type<Value, Args...>;

// Variable template to value
template <template <auto, class...> class Trait, auto Value, class... Args>
inline constexpr auto nttp_trait_v
= nttp_trait<Trait>::template type<Value, Args...>::value;

// Alias template to enable if
template <template <auto, class...> class Trait, auto Value, class... Args>
using if_nttp_trait_t
= std::enable_if_t<nttp_trait_v<Trait, Value, Args...>>;

// Makes a wrapper that holds a nttp trait
template <template <auto, class...> class Trait>
constexpr nttp_trait<Trait> wrap_trait() noexcept {
    return nttp_trait<Trait>{};
}

// Applies a wrapped nttp trait on a wrapped nttp
template <template <auto, class...> class Trait, auto Value, class... Args>
struct apply_trait<nttp_trait<Trait>, nttp_wrapper<Value>, Args...> {
    using type = typename Trait<Value, Args...>::type;
    static constexpr auto value = Trait<Value, Args...>::value;
};
// ========================================================================== //



// =============================== TYPE TRAIT =============================== //
// A wrapper to hold a trait on a type
template <template <class, class...> class Trait>
struct type_trait: trait_wrapper_base {
    using wrapper_type = type_trait<Trait>;
    template <template <class, class...> class Other>
    using rebind_wrapper = type_trait<Other>;
    template <class Type, class... Args>
    using temploid = Trait<Type, Args...>;
    template <class Type, class... Args>
    using type = typename Trait<Type, Args...>::type;
    template <class Type, class... Args>
    static constexpr auto value = Trait<Type, Args...>::value;
};

// Alias template to type
template <template <class, class...> class Trait, class Type, class... Args>
using type_trait_t
= typename type_trait<Trait>::template type<Type, Args...>;

// Variable template to value
template <template <class, class...> class Trait, class Type, class... Args>
inline constexpr auto type_trait_v
= type_trait<Trait>::template type<Type, Args...>::value;

// Alias template to enable if
template <template <class, class...> class Trait, class Type, class... Args>
using if_type_trait_t
= std::enable_if_t<type_trait_v<Trait, Type, Args...>>;

// Makes a wrapper that holds a type trait
template <template <class, class...> class Trait>
constexpr type_trait<Trait> wrap_trait() noexcept {
    return type_trait<Trait>{};
}

// Applies a wrapped type trait on a wrapped type
template <template <class, class...> class Trait, class Type, class... Args>
struct apply_trait<type_trait<Trait>, type_wrapper<Type>, Args...> {
    using type = typename Trait<Type, Args...>::type;
    static constexpr auto value = Trait<Type, Args...>::value;
};
// ========================================================================== //



// =========================== NTTP TEMPLATE TRAIT ========================== //
// A wrapper to hold a trait on a nttp template
template <template <template <auto...> class, class...> class Trait>
struct nttp_template_trait: trait_wrapper_base {
    using wrapper_type = nttp_template_trait<Trait>;
    template <template <template <auto...> class, class...> class Other>
    using rebind_wrapper = nttp_template_trait<Other>;
    template <template <auto...> class Template, class... Args>
    using temploid = Trait<Template, Args...>;
    template <template <auto...> class Template, class... Args>
    using type = typename Trait<Template, Args...>::type;
    template <template <auto...> class Template, class... Args>
    static constexpr auto value = Trait<Template, Args...>::value;
};

// Alias template to type
template <
    template <template <auto...> class, class...> class Trait,
    template <auto...> class Template,
    class... Args
>
using nttp_template_trait_t
= typename nttp_template_trait<Trait>::template type<Template, Args...>;

// Variable template to value
template <
    template <template <auto...> class, class...> class Trait,
    template <auto...> class Template,
    class... Args
>
inline constexpr auto nttp_template_trait_v
= nttp_template_trait<Trait>::template type<Template, Args...>::value;

// Alias template to enable if
template <
    template <template <auto...> class, class...> class Trait,
    template <auto...> class Template,
    class... Args
>
using if_nttp_template_trait_t
= std::enable_if_t<nttp_template_trait_v<Trait, Template, Args...>>;

// Makes a wrapper that holds a nttp template trait
template <template <template <auto...> class, class...> class Trait>
constexpr nttp_template_trait<Trait> wrap_trait() noexcept {
    return nttp_template_trait<Trait>{};
}

// Applies a wrapped nttp template trait on a wrapped nttp template
template <
    template <template <auto...> class, class...> class Trait,
    template <auto...> class Template,
    class... Args
>
struct apply_trait<
    nttp_template_trait<Trait>,
    nttp_template_wrapper<Template>,
    Args...
> {
    using type = typename Trait<Template, Args...>::type;
    static constexpr auto value = Trait<Template, Args...>::value;
};
// ========================================================================== //



// =========================== TYPE TEMPLATE TRAIT ========================== //
// A wrapper to hold a trait on a type template
template <template <template <class...> class, class...> class Trait>
struct type_template_trait: trait_wrapper_base {
    using wrapper_type = type_template_trait<Trait>;
    template <template <template <class...> class, class...> class Other>
    using rebind_wrapper = type_template_trait<Other>;
    template <template <class...> class Template, class... Args>
    using temploid = Trait<Template, Args...>;
    template <template <class...> class Template, class... Args>
    using type = typename Trait<Template, Args...>::type;
    template <template <class...> class Template, class... Args>
    static constexpr auto value = Trait<Template, Args...>::value;
};

// Alias template to type
template <
    template <template <class...> class, class...> class Trait,
    template <class...> class Template,
    class... Args
>
using type_template_trait_t
= typename type_template_trait<Trait>::template type<Template, Args...>;

// Variable template to value
template <
    template <template <class...> class, class...> class Trait,
    template <class...> class Template,
    class... Args
>
inline constexpr auto type_template_trait_v
= type_template_trait<Trait>::template type<Template, Args...>::value;

// Alias template to enable if
template <
    template <template <class...> class, class...> class Trait,
    template <class...> class Template,
    class... Args
>
using if_type_template_trait_t
= std::enable_if_t<type_template_trait_v<Trait, Template, Args...>>;

// Makes a wrapper that holds a type template trait
template <template <template <class...> class, class...> class Trait>
constexpr type_template_trait<Trait> wrap_trait() noexcept {
    return type_template_trait<Trait>{};
}

// Applies a wrapped type template trait on a wrapped type template
template <
    template <template <class...> class, class...> class Trait,
    template <class...> class Template,
    class... Args
>
struct apply_trait<
    type_template_trait<Trait>,
    type_template_wrapper<Template>,
    Args...
> {
    using type = typename Trait<Template, Args...>::type;
    static constexpr auto value = Trait<Template, Args...>::value;
};
// ========================================================================== //



// ========================== MIXED TEMPLATE TRAIT ========================== //
// A wrapper to hold a trait on a mixed template
template <template <template <class, auto...> class, class...> class Trait>
struct mixed_template_trait: trait_wrapper_base {
    using wrapper_type = mixed_template_trait<Trait>;
    template <template <template <class, auto...> class, class...> class Other>
    using rebind_wrapper = mixed_template_trait<Other>;
    template <template <class, auto...> class Template, class... Args>
    using temploid = Trait<Template, Args...>;
    template <template <class, auto...> class Template, class... Args>
    using type = typename Trait<Template, Args...>::type;
    template <template <class, auto...> class Template, class... Args>
    static constexpr auto value = Trait<Template, Args...>::value;
};

// Alias template to type
template <
    template <template <class, auto...> class, class...> class Trait,
    template <class, auto...> class Template,
    class... Args
>
using mixed_template_trait_t
= typename mixed_template_trait<Trait>::template type<Template, Args...>;

// Variable template to value
template <
    template <template <class, auto...> class, class...> class Trait,
    template <class, auto...> class Template,
    class... Args
>
inline constexpr auto mixed_template_trait_v
= mixed_template_trait<Trait>::template type<Template, Args...>::value;

// Alias template to enable if
template <
    template <template <class, auto...> class, class...> class Trait,
    template <class, auto...> class Template,
    class... Args
>
using if_mixed_template_trait_t
= std::enable_if_t<mixed_template_trait_v<Trait, Template, Args...>>;

// Makes a wrapper that holds a mixed template trait
template <template <template <class, auto...> class, class...> class Trait>
constexpr mixed_template_trait<Trait> wrap_trait() noexcept {
    return mixed_template_trait<Trait>{};
}

// Applies a wrapped mixed template trait on a wrapped mixed template
template <
    template <template <class, auto...> class, class...> class Trait,
    template <class, auto...> class Template,
    class... Args
>
struct apply_trait<
    mixed_template_trait<Trait>,
    mixed_template_wrapper<Template>,
    Args...
> {
    using type = typename Trait<Template, Args...>::type;
    static constexpr auto value = Trait<Template, Args...>::value;
};
// ========================================================================== //



// =========================== IS VARIABLE WRAPPER ========================== //
// Checks if the type is a variable wrapper: not a variable wrapper
template <class T, class = void>
struct is_variable_wrapper: std::false_type {};

// Checks if the type is a variable wrapper: it is a variable wrapper
template <class Wrapper>
struct is_variable_wrapper<
    Wrapper,
    std::enable_if_t<Wrapper::is_variable_wrapper::value>
>: std::true_type {};

// Variable template
template <class T>
inline constexpr bool is_variable_wrapper_v = is_variable_wrapper<T>::value;

// Alias template
template <class T>
using if_variable_wrapper_t = std::enable_if_t<is_variable_wrapper_v<T>>;
// ========================================================================== //



// ========================== VARIABLE WRAPPER BASE ========================= //
// The common base of variable wrappers
struct variable_wrapper_base: wrapper_base {
    using is_variable_wrapper = std::true_type;
};
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _WRAPPERS_HPP_INCLUDED
// ========================================================================== //
