//
// Created by WMJ on 2022/6/7.
//

#ifndef REMOTESIGNALSLOT_UTILS_H
#define REMOTESIGNALSLOT_UTILS_H

#include <string>
#include <sstream>
#include <mutex>
#include <type_traits>
#include <memory>

namespace RSL {
    std::string generateUUID(const std::string& prefix="");

    unsigned inc();

    template<typename ...Args>
    std::string generateID(Args... args) {
        static_assert(sizeof...(Args) > 0);
        std::stringstream ss;
        auto wrapper = [&ss](const auto& arg) {
            ss << arg;
            return "-";
        };
        (ss << ... << wrapper(args));
        ss << inc();
        return ss.str();
    }



    namespace trait {

/// represent a list of types
        template <typename...> struct typelist {};

/**
 * Pointers that can be converted to a weak pointer concept for tracking
 * purpose must implement the to_weak() function in order to make use of
 * ADL to convert that type and make it usable
 */

        template <typename T>
        std::weak_ptr<T> to_weak(std::weak_ptr<T> w) {
            return w;
        }

        template <typename T>
        std::weak_ptr<T> to_weak(std::shared_ptr<T> s) {
            return s;
        }

        // tools
        namespace detail {

            template <typename...>
            struct voider { using type = void; };

            // void_t from c++17
            template <typename...T>
            using void_t = typename detail::voider<T...>::type;

            template <typename, typename = void>
            struct has_call_operator : std::false_type {};

            template <typename F>
            struct has_call_operator<F, void_t<decltype(&std::remove_reference<F>::type::operator())>>
                    : std::true_type {};

            template <typename, typename, typename = void, typename = void>
            struct is_callable : std::false_type {};

            template <typename F, typename P, typename... T>
            struct is_callable<F, P, typelist<T...>,
            void_t<decltype(((*std::declval<P>()).*std::declval<F>())(std::declval<T>()...))>>
            : std::true_type {};

        template <typename F, typename... T>
        struct is_callable<F, typelist<T...>,
                void_t<decltype(std::declval<F>()(std::declval<T>()...))>>
        : std::true_type {};


    template <typename T, typename = void>
    struct is_weak_ptr : std::false_type {};

    template <typename T>
    struct is_weak_ptr<T, void_t<decltype(std::declval<T>().expired()),
            decltype(std::declval<T>().lock()),
            decltype(std::declval<T>().reset())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_weak_ptr_compatible : std::false_type {};

template <typename T>
struct is_weak_ptr_compatible<T, void_t<decltype(to_weak(std::declval<T>()))>>
: is_weak_ptr<decltype(to_weak(std::declval<T>()))> {};

} // namespace detail


/// determine if a pointer is convertible into a "weak" pointer
template <typename P>
constexpr bool is_weak_ptr_compatible_v = detail::is_weak_ptr_compatible<std::decay_t<P>>::value;

/// determine if a type T (Callable or Pmf) is callable with supplied arguments
template <typename L, typename... T>
constexpr bool is_callable_v = detail::is_callable<T..., L>::value;

template <typename T>
constexpr bool is_weak_ptr_v = detail::is_weak_ptr<T>::value;

template <typename T>
constexpr bool has_call_operator_v = detail::has_call_operator<T>::value;

template <typename T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;

template <typename T>
constexpr bool is_func_v = std::is_function<T>::value;

template <typename T>
constexpr bool is_pmf_v = std::is_member_function_pointer<T>::value;

//template <typename T>
//constexpr bool is_observer_v = std::is_base_of<::sigslot::detail::observer_type,
//        std::remove_pointer_t<std::remove_reference_t<T>>>::value;

} // namespace trait

}

#endif //REMOTESIGNALSLOT_UTILS_H
