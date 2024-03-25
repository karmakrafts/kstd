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
    template<typename T>
    [[nodiscard]] constexpr T* align_pointer(T* pointer, const usize alignment) noexcept {
        return reinterpret_cast<T*>((reinterpret_cast<usize>(pointer) + alignment - 1) & ~alignment);
    }

    template<typename T>
    [[nodiscard]] constexpr remove_ref<T>&& move(T&& value) noexcept {
        return static_cast<remove_ref<T>&&>(value);
    }

    template<typename T>
    [[nodiscard]] constexpr T&& forward(remove_ref<T>& value) noexcept {
        return static_cast<T&&>(value);
    }

    template<typename T>
    [[nodiscard]] constexpr T&& forward(remove_ref<T>&& value) noexcept {
        return static_cast<T&&>(value);
    }
}// namespace kstd