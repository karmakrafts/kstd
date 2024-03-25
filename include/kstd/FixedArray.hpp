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
    template<typename T, usize TSize>
    struct FixedArray final {
        using element_type = T;
        using slice_type = Slice<element_type>;
        static constexpr usize size = TSize;

    private:
        element_type _data[size];

        template<usize TIndex, element_type THead, element_type... TTail>
        constexpr auto set_all_impl() noexcept -> void {
            _data[TIndex] = THead;
            if constexpr(sizeof...(TTail) > 0) {
                set_all_impl<TIndex + 1, TTail...>();
            }
        }

        template<usize TIndex, typename THead, typename... TTail>
        requires(is_convertible<T, THead>)
        constexpr auto set_all_impl(THead&& head, TTail&&... tail) noexcept -> void {
            _data[TIndex] = head;
            if constexpr(sizeof...(tail) > 0) {
                set_all_impl<TIndex + 1, TTail...>(forward<TTail>(tail)...);
            }
        }

        template<typename THead, typename... TTail>
        requires(is_convertible<T, THead>)
        constexpr auto set_all_impl(const usize offset, THead&& head, TTail&&... tail) noexcept -> void {
            _data[offset] = head;
            if constexpr(sizeof...(tail) > 0) {
                set_all_impl<TTail...>(offset + 1, forward<TTail>(tail)...);
            }
        }

    public:
        constexpr FixedArray() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(FixedArray, FixedArray, constexpr)
        ~FixedArray() noexcept = default;

        // NOLINTBEGIN
        FixedArray(const slice_type& slice) noexcept {
            slice.copy_to(_data, min(slice.get_size(), size));
        }

        operator slice_type() const noexcept {
            return {_data, size};
        }

        // NOLINTEND

        template<usize TOffset, element_type... TValues>
        constexpr auto set_all() noexcept -> void {
            set_all_impl<TOffset, TValues...>();
        }

        template<usize TOffset, typename... TArgs>
        constexpr auto set_all(TArgs&&... values) noexcept -> void {
            set_all_impl<TOffset, TArgs...>(forward<TArgs>(values)...);
        }

        template<typename... TArgs>
        constexpr auto set_all(const usize offset, TArgs&&... values) noexcept -> void {
            set_all_impl<TArgs...>(offset, forward<TArgs>(values)...);
        }

        [[nodiscard]] auto get_data() noexcept -> element_type* {
            return _data;
        }

        [[nodiscard]] auto get_const_data() const noexcept -> const element_type* {
            return _data;
        }

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return size;
        }

        [[nodiscard]] constexpr auto operator[](const usize index) noexcept -> element_type& {
            return _data[index];
        }

        [[nodiscard]] constexpr auto operator[](const usize index) const noexcept -> const element_type& {
            return _data[index];
        }
    };

    template<decltype(auto)... TValues, typename T = PackElement<0, decltype(TValues)...>>
    requires(are_convertible<T, decltype(TValues)...>)
    [[nodiscard]] constexpr auto fixed_array_of() noexcept -> FixedArray<T, sizeof...(TValues)> {
        FixedArray<T, sizeof...(TValues)> array {};
        array.template set_all<0, TValues...>();
        return array;
    }

    template<typename... TArgs, typename T = PackElement<0, TArgs...>>
    requires(are_convertible<T, TArgs...>)
    [[nodiscard]] constexpr auto fixed_array_of(TArgs&&... args) noexcept -> FixedArray<T, sizeof...(TArgs)> {
        FixedArray<T, sizeof...(TArgs)> array {};
        array.template set_all<0>(forward<TArgs>(args)...);
        return array;
    }
}// namespace kstd