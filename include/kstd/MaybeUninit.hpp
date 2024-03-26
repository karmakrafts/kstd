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

#include "FixedArray.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace kstd {
    template<typename T>
    struct MaybeUninit final {
        using value_type = T;

    private:
        using self_type = MaybeUninit<T>;

        FixedArray<u8, sizeof(T), alignof(T)> _data;

    public:
        constexpr MaybeUninit() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(MaybeUninit, MaybeUninit, constexpr);
        ~MaybeUninit() noexcept = default;

        auto operator=(T&& value) noexcept -> self_type {
            *reinterpret_cast<T*>(_data.data()) = forward<T>(value);
            return *this;
        }

        auto operator=(const T& value) noexcept -> self_type {
            *reinterpret_cast<T*>(_data.data()) = value;
            return *this;
        }

        // This may not be used as the right hand side of an assignment to T
        operator T&&() noexcept = delete;

        [[nodiscard]] operator T&() noexcept {
            return *reinterpret_cast<T*>(_data.data());
        }

        [[nodiscard]] operator const T&() const noexcept {
            return *reinterpret_cast<const T*>(_data.data());
        }
    };
}// namespace kstd