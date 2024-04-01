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

#include <fmt/format.h>

#include "Concepts.hpp"
#include "Allocator.hpp"
#include "Defaults.hpp"

namespace kstd {
    template<concepts::Integer T, concepts::Allocator<T> TAllocator = Allocator<T>>
    struct BasicString final {
        using value_type = T;
        using allocator_type = TAllocator;

    private:
        using self_type = BasicString<T, TAllocator>;
    };

    template<concepts::Allocator<char> TAllocator = Allocator<char>>
    using String = BasicString<char, TAllocator>;

    template<concepts::Allocator<wchar_t> TAllocator = Allocator<wchar_t>>
    using WString = BasicString<wchar_t, TAllocator>;
}// namespace kstd