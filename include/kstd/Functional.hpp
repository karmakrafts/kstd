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

#include "Meta.hpp"

namespace kstd {
    // is_callable
    template<typename T, typename... TArgs>
    struct IsCallable {
        // ReSharper disable once CppFunctionIsNotImplemented
        template<typename _T, typename... _TArgs>
        static auto test(int) -> decltype(declval<_T&&>()(declval<_TArgs&&>()...), TrueType {});

        // ReSharper disable once CppFunctionIsNotImplemented
        template<typename, typename...>
        static auto test(...) -> FalseType;

        static constexpr bool value = decltype(test<T, TArgs...>(0))::value;
    };

    template<typename T, typename... TArgs>
    constexpr bool is_callable = IsCallable<T, TArgs...>::value;

    // is_callable_r
    template<typename T, typename TReturn, typename... TArgs>
    struct IsCallableR {
        static_assert(is_callable<T, TArgs...>, "Type is not callable with the given argument types");
        static constexpr bool value = is_same<TReturn, decltype(declval<T&&>()(declval<TArgs&&>()...))>;
    };

    template<typename T, typename TReturn, typename... TArgs>
    constexpr bool is_callable_r = IsCallableR<T, TReturn, TArgs...>::value;
}// namespace kstd