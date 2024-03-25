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

#include <kstd/Functional.hpp>

using namespace kstd;

auto foo(int, bool, float) noexcept -> void {
}

static_assert(is_callable<decltype(foo), int, bool, float>);
static_assert(is_callable_r<decltype(foo), void, int, bool, float>);

auto bar(int) noexcept -> void* {
    return nullptr;
}

static_assert(is_callable_r<decltype(bar), void*, int>);