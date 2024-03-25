// Copyright 2024 Karma Krafts & associates
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Types.hpp"

namespace kstd {
    template<auto TValue>
    struct ConstantType {
        static constexpr decltype(TValue) value = TValue;
    };

    using TrueType = ConstantType<true>;
    using FalseType = ConstantType<false>;

    template<typename T>
    [[noreturn]] [[nodiscard]] constexpr T&& declval() noexcept {
        throw;
    }

    // RemoveRef
    template<typename T>
    struct RemoveRef {
        using type = T;
    };

    template<typename T>
    struct RemoveRef<T&> {
        using type = T;
    };

    template<typename T>
    struct RemoveRef<T&&> {
        using type = T;
    };

    template<typename T>
    using remove_ref = typename RemoveRef<T>::type;

    // RemovePtr
    template<typename T>
    struct RemovePtr {
        using type = T;
    };

    template<typename T>
    struct RemovePtr<T*> {
        using type = T;
    };

    template<typename T>
    using remove_ptr = typename RemovePtr<T>::type;

    // Conditional
    template<bool, typename, typename>
    struct Conditional;

    template<typename TTrue, typename TFalse>
    struct Conditional<true, TTrue, TFalse> {
        using type = TTrue;
    };

    template<typename TTrue, typename TFalse>
    struct Conditional<false, TTrue, TFalse> {
        using type = TFalse;
    };

    template<bool TCondition, typename TTrue, typename TFalse>
    using conditional = typename Conditional<TCondition, TTrue, TFalse>::type;

    // Void
    template<typename...>
    struct ToVoid {
        using type = void;
    };

    template<typename... TTypes>
    using to_void = typename ToVoid<TTypes...>::type;

    // IsDirectConstructible
    template<typename T, typename... TArgs>
    struct IsDirectConstructible {
        template<typename U, decltype(U {declval<TArgs>()...})* = nullptr>
        static char test(int);

        template<typename U>
        static long test(...);

        static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template<typename T, typename... TArgs>
    constexpr bool is_direct_constructible = IsDirectConstructible<T, TArgs...>::value;

    // EnableIf
    template<bool, typename = void>
    struct EnableIf {};

    template<typename T>
    struct EnableIf<true, T> {
        using type = T;
    };

    template<bool TCondition, typename T = void>
    using enable_if = typename EnableIf<TCondition, T>::type;

    // RemoveConst
    template<typename T, typename = void>
    struct RemoveConst {
        using type = T;
    };

    template<typename T>
    struct RemoveConst<const T> {
        using type = T;
    };

    template<typename T>
    struct RemoveConst<const T*> {
        using type = T*;
    };

    template<typename T>
    struct RemoveConst<const T&> {
        using type = T&;
    };

    template<typename T>
    using remove_const = typename RemoveConst<T>::type;

    // IsSame
    template<typename, typename>
    struct IsSame : FalseType {};

    template<typename T>
    struct IsSame<T, T> : TrueType {};

    template<typename TFirst, typename TSecond>
    constexpr bool is_same = IsSame<TFirst, TSecond>::value;

    // Decay
    template<typename T>
    struct Decay {
        using type = T;
    };

    template<typename T>
    struct Decay<T const> {
        using type = typename Decay<T>::type;
    };

    template<typename T>
    struct Decay<T*> {
        using type = typename Decay<T>::type;
    };

    template<typename T>
    struct Decay<T&> {
        using type = typename Decay<T>::type;
    };

    template<typename T>
    struct Decay<T&&> {
        using type = typename Decay<T>::type;
    };

    template<typename T>
    using decay = typename Decay<T>::type;

    // IsComplete
    template<typename, typename = void>
    struct IsComplete : FalseType {};

    template<typename T>
    struct IsComplete<T, to_void<decltype(sizeof(T) != 0)>> : TrueType {};

    template<typename T>
    constexpr bool is_complete = IsComplete<T>::value;

    // IsAssignable
    template<typename, typename, typename = void>
    struct IsAssignable : FalseType {};

    template<typename TLeft, typename TRight>
    struct IsAssignable<TLeft, TRight, to_void<decltype(declval<TLeft&>() = declval<TRight&&>())>> : TrueType {};

    template<typename TLeft, typename TRight>
    constexpr bool is_assignable = IsAssignable<TLeft, TRight>::value;

    // IsConvertible
    template<typename TTo, typename TFrom>
    struct IsConvertible {
        template<typename T, typename F>
        static auto test(T) -> decltype(static_cast<T>(declval<F>()), TrueType{});

        template<typename, typename>
        static auto test(...) -> FalseType;

        static constexpr bool value = decltype(test<TTo, TFrom>(0))::value;
    };

    template<>
    struct IsConvertible<void, void> : TrueType {};

    template<typename TTo, typename TFrom>
    constexpr bool is_convertible = IsConvertible<TTo, TFrom>::value;

    // AreConvertible
    template<typename T, typename... TTypes>
    constexpr bool are_convertible = (is_convertible<T, TTypes> && ...);

    // AreAssignable
    template<typename T, typename... TTypes>
    constexpr bool are_assignable = (is_assignable<T, TTypes> && ...);

    // IsOneOf
    template<typename T, typename... TTypes>
    constexpr bool is_one_of = (is_same<T, TTypes> || ...);

    // AreSameAs
    template<typename T, typename... TTypes>
    constexpr bool are_same = (is_same<T, TTypes> && ...);

    // IsFloatingPoint
    template<typename T>
    constexpr bool is_floating_point = is_one_of<T, f32, f64, long double>;

    // IsSignedIntegral
    template<typename T>
    constexpr bool is_signed_integral = is_one_of<T, i8, i16, i32, i64>;

    // IsUnsignedIntegral
    template<typename T>
    constexpr bool is_unsigned_integral = is_one_of<T, u8, u16, u32, u64>;

    // IsChar
    template<typename T>
    constexpr bool is_char = is_one_of<T, char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;

    // IsIntegral
    template<typename T>
    constexpr bool is_integral = is_char<T> || is_signed_integral<T> || is_unsigned_integral<T>;
}// namespace kstd