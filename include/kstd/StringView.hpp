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

#include "Concepts.hpp"
#include "Defaults.hpp"
#include "Slice.hpp"
#include "Types.hpp"

namespace kstd {
    template<concepts::Integer T>
    struct BasicStringView final {
        using char_type = T;
        using slice_type = Slice<char_type>;

    private:
        using self_type = BasicStringView<char_type>;

        const char_type* _data {};
        usize _length {};

    public:
        constexpr BasicStringView() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(BasicStringView, self_type, constexpr)
        ~BasicStringView() noexcept = default;

        [[nodiscard]] auto get_data() const noexcept -> const char_type* {
            return _data;
        }

        [[nodiscard]] auto get_length() const noexcept -> usize {
            return _length;
        }
    };

    using StringView = BasicStringView<char>;
}// namespace kstd