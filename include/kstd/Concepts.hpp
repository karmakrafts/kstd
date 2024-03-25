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

namespace kstd::concepts {
    template<typename T>
    concept Char = is_char<T>;

    template<typename T>
    concept Signed = is_signed_integral<T>;

    template<typename T>
    concept Unsigned = is_unsigned_integral<T>;

    template<typename T>
    concept Integer = is_integral<T>;

    template<typename T>
    concept Float = is_floating_point<T>;

    template<typename TAs, typename T>
    concept AssignableAs = is_assignable<T, TAs>;

    template<typename TTo, typename T>
    concept ConvertibleTo = is_convertible<TTo, T>;
}// namespace kstd