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

#include "System.hpp"
#include "Types.hpp"

namespace kstd {
    template<typename T>
    struct SystemAllocator final {
        using value_type = T;
        static constexpr bool has_state = false;

        SystemAllocator() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(SystemAllocator, SystemAllocator)
        ~SystemAllocator() noexcept = default;

        [[nodiscard]] auto allocate(const usize count) noexcept -> value_type* {
            return static_cast<value_type*>(::malloc(count * sizeof(value_type)));
        }

        [[nodiscard]] auto reallocate(value_type* memory, [[maybe_unused]] const usize old_count, const usize count) noexcept
            -> value_type* {
            return static_cast<value_type*>(::realloc(memory, count * sizeof(value_type)));
        }

        auto free(value_type* memory) noexcept -> void {
            ::free(memory);
        }
    };
}// namespace kstd