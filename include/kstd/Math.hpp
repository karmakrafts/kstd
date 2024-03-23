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

namespace kstd {
    template<typename T>
    constexpr auto min(T a, T b) noexcept -> T {
        return a < b ? a : b;
    }

    template<typename T>
    constexpr auto max(T a, T b) noexcept -> T {
        return a > b ? a : b;
    }

    template<typename T>
    constexpr auto clamp(T x, T min, T max) noexcept -> T {
        if(x < min) {
            return min;
        }
        if(x > max) {
            return max;
        }
        return x;
    }
}// namespace kstd