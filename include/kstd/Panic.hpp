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

// NOLINTBEGIN
#include <stdio.h>
#include <stdlib.h>
// NOLINTEND

#include "SourceLocation.hpp"
#include "StackTrace.hpp"

namespace kstd {
#ifdef KSTD_STACKTRACE
    [[noreturn]] inline void panic(const char* message, KSTD_LOCATION) noexcept {
        const auto trace = StackTrace::get_current();
    }
#else
    [[noreturn]] inline void panic(const char* message, KSTD_LOCATION) noexcept {
        ::printf("Panic at %s:%s:%lu: %s\n", __loc.get_file(), __loc.get_function(), __loc.get_line(), message);
        ::exit(1);
    }
#endif
}// namespace kstd

#define KSTD_PANIC(message) kstd::panic(message, __loc)