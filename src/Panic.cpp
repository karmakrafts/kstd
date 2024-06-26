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

#include "kstd/Panic.hpp"

// NOLINTBEGIN
#include <stdio.h>
#include <stdlib.h>
// NOLINTEND

#include "kstd/StackTrace.hpp"

namespace kstd {
    auto panic(const char* message) noexcept -> void {
        const auto trace = StackTrace::get_current(100, 2);
        exit(1);
    }
}// namespace kstd