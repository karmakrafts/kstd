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

#include <atomic>

#include "Types.hpp"

namespace kstd {
    // TODO: Replace these
    template<typename T>
    using Atomic = std::atomic<T>;

    using atomic_i8 = Atomic<i8>;
    using atomic_i16 = Atomic<i16>;
    using atomic_i32 = Atomic<i32>;
    using atomic_i64 = Atomic<i64>;

    using atomic_u8 = Atomic<u8>;
    using atomic_u16 = Atomic<u16>;
    using atomic_u32 = Atomic<u32>;
    using atomic_u64 = Atomic<u64>;

    using atomic_bool = Atomic<bool>;
}// namespace kstd