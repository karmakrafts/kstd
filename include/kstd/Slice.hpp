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

#include "Defaults.hpp"
#include "System.hpp"
#include "Types.hpp"

namespace kstd {
    template<typename T>
    struct Slice final {
        using value_type = T;
        using const_iterator = const value_type*;

    private:
        using self_type = Slice<value_type>;

        const T* const _data;
        usize _size;

    public:
        Slice(const T* data, const usize size) noexcept
            : _data(data)
            , _size(size) {
        }

        KSTD_DEFAULT_MOVE_COPY(Slice, self_type, inline)
        ~Slice() noexcept = default;

        [[nodiscard]] auto begin() const noexcept -> const_iterator {
            return _data;
        }

        [[nodiscard]] auto end() const noexcept -> const_iterator {
            return _data + _size;
        }

        [[nodiscard]] auto size() const noexcept -> usize {
            return _size;
        }

        [[nodiscard]] auto data() noexcept -> T* {
            return _data;
        }

        [[nodiscard]] auto data() const noexcept -> const T* {
            return _data;
        }

        [[nodiscard]] auto sub_slice(const usize start, const usize end) const noexcept -> self_type {
            return {_data + start, end - start};
        }

        auto copy_to(T* dst, const usize size) const noexcept -> void {
            memcpy(dst, _data, sizeof(T) * size);
        }

        auto copy_to_unsafe(void* dst, const usize size) const noexcept -> void {
            memcpy(dst, _data, size);
        }
    };
}// namespace kstd