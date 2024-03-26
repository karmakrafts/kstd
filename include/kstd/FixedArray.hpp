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
#include "Math.hpp"
#include "ParameterPack.hpp"
#include "Slice.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace kstd {
    template<typename T, usize TSize, usize TAlignment = alignof(T)>
    struct alignas(TAlignment) FixedArray {
        using element_type = T;
        using slice_type = Slice<element_type>;
        static constexpr usize buffer_size = TSize;
        static constexpr usize buffer_alignment = TAlignment;

    private:
        element_type _data[buffer_size];

        template<typename THead, typename... TTail>
        requires(is_convertible<T, THead>)
        constexpr auto insert_all_impl(const usize offset, THead&& head, TTail&&... tail) noexcept -> void {
            _data[offset] = head;
            if constexpr(sizeof...(tail) > 0) {
                insert_all_impl<TTail...>(offset + 1, forward<TTail>(tail)...);
            }
        }

    public:
        constexpr FixedArray() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(FixedArray, FixedArray, constexpr)
        ~FixedArray() noexcept = default;

        FixedArray(const slice_type& slice) noexcept {
            slice.copy_to(_data, min(slice.size(), buffer_size));
        }

        operator slice_type() const noexcept {
            return {_data, buffer_size};
        }

        template<typename... TArgs>
        constexpr auto insert_all(TArgs&&... values) noexcept -> void {
            insert_all_impl<TArgs...>(0, forward<TArgs>(values)...);
        }

        template<typename... TArgs>
        constexpr auto insert_all_at(const usize index, TArgs&&... values) noexcept -> void {
            insert_all_impl<TArgs...>(index, forward<TArgs>(values)...);
        }

        [[nodiscard]] auto data() noexcept -> element_type* {
            return _data;
        }

        [[nodiscard]] auto data() const noexcept -> const element_type* {
            return _data;
        }

        [[nodiscard]] constexpr auto size() const noexcept -> usize {
            return buffer_size;
        }

        [[nodiscard]] constexpr auto alignment() const noexcept -> usize {
            return buffer_alignment;
        }

        [[nodiscard]] constexpr auto operator[](const usize index) noexcept -> element_type& {
            return _data[index];
        }

        [[nodiscard]] constexpr auto operator[](const usize index) const noexcept -> const element_type& {
            return _data[index];
        }
    };

    template<typename... TArgs, typename T = PackElement<0, TArgs...>>
    requires(are_assignable<T, TArgs...>)
    [[nodiscard]] constexpr auto fixed_array_of(TArgs&&... args) noexcept -> FixedArray<T, sizeof...(TArgs)> {
        FixedArray<T, sizeof...(TArgs)> array {};
        array.insert_all(forward<TArgs>(args)...);
        return array;
    }
}// namespace kstd