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
template <class...>
struct variable_wrapper_base: wrapper_base {
    using is_variable_wrapper = std::true_type;
};

// The common base of variable wrappers: using CRTP
template <template <class...> class CRTP, class T>
class variable_wrapper_base<CRTP<T>>: public variable_wrapper_base<>
{
    // Types
    public:
    using wrapper_type = CRTP<T>;
    template <class U>
    using rebind_wrapper = CRTP<U>;
    using type = T;

    // Helpers
    private:
    using self = variable_wrapper_base<CRTP<T>>;
    using crtp = CRTP<T>;
    template <class>
    struct is_same_crtp: std::false_type {};
    template <class U>
    struct is_same_crtp<CRTP<U>>: std::true_type {};
    template <class U>
    static constexpr bool is_same_crtp_v = is_same_crtp<U>::value;
    template <class U>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<U>>;
    template <bool B>
    using if_t = std::enable_if_t<B, std::bool_constant<B>>;
    template <class U, class = void>
    struct is_wrapper {};
    template <class U>
    struct is_wrapper<U, std::enable_if_t<
        is_variable_wrapper_v<remove_cvref_t<U>> &&
        !std::is_same_v<remove_cvref_t<U>, remove_cvref_t<crtp>>,
        std::enable_if_t<std::is_same_v<
            typename remove_cvref_t<U>::type,
            remove_cvref_t<decltype(std::declval<U>()())>
        >>
    >>: std::enable_if<std::true_type::value, decltype(std::declval<U>()())> {};
    template <class U>
    using if_wrapper = typename is_wrapper<U>::type;
    template <class U, class = void>
    struct is_not_wrapper {};
    template <class U>
    struct is_not_wrapper<U, std::enable_if_t<
        !is_variable_wrapper_v<remove_cvref_t<U>>
    >>: std::enable_if<std::true_type::value, U> {};
    template <class U>
    using if_not_wrapper = typename is_not_wrapper<U>::type;
    template <class... U>
    using if_constructible = if_t<std::is_constructible_v<T, U...>>;
    template <class... U>
    static constexpr bool is_nothrow_constructible
    = std::is_nothrow_constructible_v<T, U...>;
    template <class U>
    using if_convertible = if_t<std::is_convertible_v<U, T>>;
    template <class U>
    static constexpr bool is_nothrow_convertible
    = std::is_nothrow_constructible_v<U, T>;
    template <class U>
    using if_assignable = if_t<std::is_assignable_v<T&, U>>;
    template <class U>
    static constexpr bool is_nothrow_assignable
    = std::is_nothrow_assignable_v<T&, U>;
    template <class U>
    using if_castable = if_t<std::is_convertible_v<T, U>>;
    template <class U>
    static constexpr bool is_nothrow_castable
    = std::is_nothrow_constructible_v<U, T>;

    // Lifecycle
    public:
    constexpr variable_wrapper_base() noexcept(
        std::is_nothrow_default_constructible_v<type>
    ) = default;
    constexpr variable_wrapper_base(const self& other) noexcept(
        std::is_nothrow_copy_constructible_v<type>
    ) = default;
    constexpr variable_wrapper_base(self&& other) noexcept(
        std::is_nothrow_move_constructible_v<type>
    ) = default;
    template <class U, class V = if_wrapper<U&&>, class = std::void_t<
        std::enable_if_t<is_same_crtp_v<remove_cvref_t<U&&>>, if_convertible<V>>
    >, bool = std::true_type::value>
    explicit constexpr variable_wrapper_base(U&& other) noexcept(
        is_nothrow_constructible<U&&>
    ): _variable(std::forward<U>(other)) {
    }
    template <class U, class = if_not_wrapper<U&&>, class = if_convertible<U&&>>
    constexpr variable_wrapper_base(U&& other) noexcept(
        is_nothrow_constructible<U&&>
    ): _variable(std::forward<U>(other)) {
    }
    template <class U, class... Us, class = std::void_t<
        std::enable_if_t<!sizeof...(Us), if_constructible<if_wrapper<U&&>>>
    >>
    explicit constexpr variable_wrapper_base(U&& other, Us&&...) noexcept(
        is_nothrow_constructible<U&&, Us&&...>
    ): _variable(std::forward<U>(other)()) {
    }
    template <class U, class... Us, class = std::void_t<
        std::void_t<if_not_wrapper<U&&>, if_constructible<U&&, Us&&...>>
    >, bool = std::true_type::value>
    explicit constexpr variable_wrapper_base(U&& other, Us&&... args) noexcept(
        is_nothrow_constructible<U&&, Us&&...>
    ): _variable(std::forward<U>(other), std::forward<Us>(args)...) {
    }

    // Assignment
    public:
    constexpr crtp& operator=(const self& other) noexcept(
        std::is_nothrow_copy_assignable_v<type>
    ) {
        _variable = other();
        return static_cast<crtp&>(*this);
    };
    constexpr crtp& operator=(self&& other) noexcept(
        std::is_nothrow_move_assignable_v<type>
    ) {
        _variable = std::move(other());
        return static_cast<crtp&>(*this);
    };
    template <class U, class V = if_wrapper<U&&>, int = if_assignable<V>::value>
    constexpr crtp& operator=(U&& other) noexcept(
        is_nothrow_assignable<V>
    ) {
        _variable = std::forward<U>(other)();
        return static_cast<crtp&>(*this);
    }
    template <class U, class = if_not_wrapper<U&&>, class = if_assignable<U&&>> 
    constexpr crtp& operator=(U&& other) noexcept(is_nothrow_assignable<U&&>) {
        _variable = std::forward<U>(other);
        return static_cast<crtp&>(*this);
    }

    // Conversion
    public:
    template <class U, class = if_not_wrapper<U>, class = if_castable<U>>
    explicit constexpr operator U() const noexcept(is_nothrow_castable<U>) {
        return static_cast<U>(_variable);
    }

    // Implicit access
    public:
    constexpr operator type&() & {
        return _variable;
    }
    constexpr operator const type&() const & {
        return _variable;
    }
    constexpr operator volatile type&() volatile & {
        return _variable;
    }
    constexpr operator const volatile type&() const volatile & {
        return _variable;
    }
    constexpr operator type&&() && {
        return _variable;
    }
    constexpr operator const type&&() const && {
        return _variable;
    }
    constexpr operator volatile type&&() volatile && {
        return _variable;
    }
    constexpr operator const volatile type&&() const volatile && {
        return _variable;
    }

    // Explicit access
    public:
    constexpr type& operator()() & {
        return _variable;
    }
    constexpr const type& operator()() const & {
        return _variable;
    }
    constexpr volatile type& operator()() volatile & {
        return _variable;
    }
    constexpr const volatile type& operator()() const volatile & {
        return _variable;
    }
    constexpr type&& operator()() && {
        return _variable;
    }
    constexpr const type&& operator()() const && {
        return _variable;
    }
    constexpr volatile type&& operator()() volatile && {
        return _variable;
    }
    constexpr const volatile type&& operator()() const volatile && {
        return _variable;
    }

    // Implementation details
    private:
    type _variable;
};
// ========================================================================== //



// ============================ VARIABLE WRAPPER ============================ //
// A wrapper to hold a variable
template <class T>
struct variable_wrapper: variable_wrapper_base<variable_wrapper<T>> {
    using variable_wrapper_base<variable_wrapper<T>>::variable_wrapper_base;
    using variable_wrapper_base<variable_wrapper<T>>::operator=;
    using variable_wrapper_base<variable_wrapper<T>>::operator();
};

// Makes a wrapper that holds an object or a reference
template <class T>
constexpr variable_wrapper<T> wrap(const T& x) noexcept {
    return variable_wrapper<T>(x);
}
// ========================================================================== //



// ============================= OBJECT WRAPPER ============================= //
// A wrapper to hold an object
template <class T>
struct object_wrapper: variable_wrapper_base<object_wrapper<T>> {
    using variable_wrapper_base<object_wrapper<T>>::variable_wrapper_base;
    using variable_wrapper_base<object_wrapper<T>>::operator=;
    using variable_wrapper_base<object_wrapper<T>>::operator();
};
    
// Makes a wrapper that holds an object
template <class T>
constexpr object_wrapper<T> wrap_object(const T& x) noexcept {
    return object_wrapper<T>(x);
}
// ========================================================================== //



// ============================ REFERENCE WRAPPER =========================== //
// A wrapper to hold a reference
template <class T>
struct reference_wrapper: variable_wrapper_base<reference_wrapper<T>> {
    using variable_wrapper_base<reference_wrapper<T>>::variable_wrapper_base;
    using variable_wrapper_base<reference_wrapper<T>>::operator=;
    using variable_wrapper_base<reference_wrapper<T>>::operator();
};
    
// Makes a wrapper that holds a reference
template <class T>
constexpr reference_wrapper<T&&> wrap_reference(T&& x) noexcept {
    return reference_wrapper<T&&>(std::forward<T>(x));
}
// ========================================================================== //



// ========================================================================== //
} // namespace epidesim
#endif // _WRAPPERS_HPP_INCLUDED
// ========================================================================== //
