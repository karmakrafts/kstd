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

#include "Types.hpp"

namespace kstd {
    template<typename... TTypes>
    struct Pack final {
        static constexpr usize size = sizeof...(TTypes);

        [[nodiscard]] constexpr auto get_size() const noexcept -> usize {
            return size;
        }
    };

    // PackElement
    template<usize TIndex, typename... TTypes>
    struct PackElementImpl;

    template<usize TIndex, typename THead, typename... TTail>
    struct PackElementImpl<TIndex, THead, TTail...> : PackElementImpl<TIndex - 1, TTail...> {};

    template<typename THead, typename... TTail>
    struct PackElementImpl<0, THead, TTail...> {
        using type = THead;
    };

    template<usize TIndex, typename... TTypes>
    using PackElement = typename PackElementImpl<TIndex, TTypes...>::type;

    // LeftTrimPack
    template<usize TCount, typename... TTypes>
    struct LeftTrimPackImpl;

    // RightTrimPack
    template<usize TCount, typename... TTypes>
    struct RightTrimPackImpl;

    // SlicePack
    template<usize TStart, usize TEnd, typename... TTypes>
    struct SlicePack;
}// namespace kstd