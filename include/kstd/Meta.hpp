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

#include "Panic.hpp"
#include "Types.hpp"

namespace kstd {
    template<auto TValue>
    struct ConstantType {
        static constexpr decltype(TValue) value = TValue;
    };

    using TrueType = ConstantType<true>;
    using FalseType = ConstantType<false>;

    template<typename T>
    [[noreturn]] [[nodiscard]] constexpr T&& DeclVal(KSTD_LOCATION) noexcept {
        KSTD_PANIC("DeclVal result may not be used at runtime");
    }

    // RemoveRef
    template<typename T>
    struct RemoveRefImpl {
        using type = T;
    };

    template<typename T>
    struct RemoveRefImpl<T&> {
        using type = T;
    };

    template<typename T>
    struct RemoveRefImpl<T&&> {
        using type = T;
    };

    template<typename T>
    using RemoveRef = typename RemoveRefImpl<T>::type;

    // RemovePtr
    template<typename T>
    struct RemovePtrImpl {
        using type = T;
    };

    template<typename T>
    struct RemovePtrImpl<T*> {
        using type = T;
    };

    template<typename T>
    using RemovePtr = typename RemovePtrImpl<T>::type;

    // VoidType
    template<typename...>
    struct MakeVoid {
        using type = void;
    };

    template<typename... TTypes>
    using VoidType = typename MakeVoid<TTypes...>::type;

    // IsDirectConstructible
    template<typename T, typename... TArgs>
    struct IsDirectConstructibleImpl {
        template<typename U, decltype(U {DeclVal<TArgs>()...})* = nullptr>
        static char test(int);

        template<typename U>
        static long test(...);

        static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template<typename T, typename... TArgs>
    constexpr bool IsDirectConstructible = IsDirectConstructibleImpl<T, TArgs...>::value;

    // EnableIf
    template<bool, typename = void>
    struct EnableIfImpl {};

    template<typename T>
    struct EnableIfImpl<true, T> {
        using type = T;
    };

    template<bool TCondition, typename T = void>
    using EnableIf = typename EnableIfImpl<TCondition, T>::type;

    // RemoveConst
    template<typename T, typename = void>
    struct RemoveConstImpl {
        using type = T;
    };

    template<typename T>
    struct RemoveConstImpl<T, const T> {
        using type = T;
    };

    template<typename T>
    using RemoveConst = typename RemoveConstImpl<T>::type;

    // IsSame
    template<typename, typename>
    struct IsSameImpl : FalseType {};

    template<typename T>
    struct IsSameImpl<T, T> : TrueType {};

    template<typename TFirst, typename TSecond>
    constexpr bool IsSame = IsSameImpl<TFirst, TSecond>::value;

    // Decay
    template<typename T>
    struct DecayImpl {
        using type = T;
    };

    template<typename T>
    struct DecayImpl<T const> {
        using type = typename DecayImpl<T>::type;
    };

    template<typename T>
    struct DecayImpl<T*> {
        using type = typename DecayImpl<T>::type;
    };

    template<typename T>
    struct DecayImpl<T&> {
        using type = typename DecayImpl<T>::type;
    };

    template<typename T>
    struct DecayImpl<T&&> {
        using type = typename DecayImpl<T>::type;
    };

    template<typename Type>
    using Decay = typename DecayImpl<Type>::type;

    // IsComplete
    template<typename, typename = void>
    struct IsCompleteImpl : FalseType {};

    template<typename T>
    struct IsCompleteImpl<T, VoidType<decltype(sizeof(T) != 0)>> : TrueType {};

    template<typename T>
    constexpr bool IsComplete = IsCompleteImpl<T>::value;

    // IsAssignable
    template<typename, typename, typename = void>
    struct IsAssignableImpl : FalseType {};

    template<typename TLeft, typename TRight>
    struct IsAssignableImpl<TLeft, TRight, VoidType<decltype(DeclVal<TLeft>() = DeclVal<TRight>())>> : TrueType {};

    template<typename TLeft, typename TRight>
    constexpr bool IsAssignable = IsAssignableImpl<TLeft, TRight>::value;

    // IsConvertible
    template<typename, typename, typename = void>
    struct IsConvertibleImpl : FalseType {};

    template<typename TTo, typename TFrom>
    struct IsConvertibleImpl<TTo, TFrom, VoidType<decltype((TTo*) DeclVal<TFrom*>())>> : TrueType {};

    template<typename TTo, typename TFrom>
    constexpr bool IsConvertible = IsConvertibleImpl<TTo, TFrom>::value;

    // AreConvertible
    template<typename T, typename... TTypes>
    constexpr bool AreConvertible = (IsConvertible<T, TTypes> && ...);

    // AreAssignable
    template<typename T, typename... TTypes>
    constexpr bool AreAssignable = (IsAssignable<T, TTypes> && ...);

    // IsOneOf
    template<typename T, typename... TTypes>
    constexpr bool IsOneOf = (IsSame<T, TTypes> || ...);

    // AreSameAs
    template<typename T, typename... TTypes>
    constexpr bool AreSame = (IsSame<T, TTypes> && ...);

    // IsFloatingPoint
    template<typename T>
    constexpr bool IsFloatingPoint = IsOneOf<T, f32, f64, long double>;

    // IsSignedIntegral
    template<typename T>
    constexpr bool IsSignedIntegral = IsOneOf<T, i8, i16, i32, i64>;

    // IsUnsignedIntegral
    template<typename T>
    constexpr bool IsUnsignedIntegral = IsOneOf<T, u8, u16, u32, u64>;

    // IsChar
    template<typename T>
    constexpr bool IsChar = IsOneOf<T, char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;

    // IsIntegral
    template<typename T>
    constexpr bool IsIntegral = IsChar<T> || IsSignedIntegral<T> || IsUnsignedIntegral<T>;
}// namespace kstd