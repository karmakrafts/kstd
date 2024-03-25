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
        using const_iterator = const char_type*;

    private:
        using self_type = BasicStringView<char_type>;

        const char_type* _data {};
        usize _length {};

    public:
        constexpr BasicStringView() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(BasicStringView, self_type, constexpr)

        constexpr BasicStringView(const char_type* data, const usize length)
            : _data(data)
            , _length(length) {
        }

        constexpr BasicStringView(const slice_type& slice) noexcept
            : _data(slice.data())
            , _length(slice.size()) {
        }

        ~BasicStringView() noexcept = default;

        [[nodiscard]] operator slice_type() const noexcept {
            return {_data, _length};
        }

        [[nodiscard]] auto cbegin() const noexcept -> const_iterator {
            return _data;
        }

        [[nodiscard]] auto cend() const noexcept -> const_iterator {
            return _data + _length;
        }

        [[nodiscard]] auto data() const noexcept -> const char_type* {
            return _data;
        }

        [[nodiscard]] auto length() const noexcept -> usize {
            return _length;
        }
    };

    using StringView = BasicStringView<char>;
    using WStringView = BasicStringView<wchar_t>;
    using StringViewUTF8 = BasicStringView<char8_t>;
    using StringViewUTF16 = BasicStringView<char16_t>;
    using StringViewUTF32 = BasicStringView<char32_t>;

    constexpr auto operator""_str(const char* value, const usize size) noexcept -> StringView {
        return {value, size};
    }

    constexpr auto operator""_str(const wchar_t* value, const usize size) noexcept -> WStringView {
        return {value, size};
    }

    constexpr auto operator""_str(const char8_t* value, const usize size) noexcept -> StringViewUTF8 {
        return {value, size};
    }

    constexpr auto operator""_str(const char16_t* value, const usize size) noexcept -> StringViewUTF16 {
        return {value, size};
    }

    constexpr auto operator""_str(const char32_t* value, const usize size) noexcept -> StringViewUTF32 {
        return {value, size};
    }
}// namespace kstd