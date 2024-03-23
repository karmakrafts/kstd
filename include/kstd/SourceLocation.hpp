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
#include "Types.hpp"

#ifndef __has_builtin
#define __has_builtin(b) 0
#endif

#if __has_builtin(__builtin_COLUMN)
#define KSTD_COLUMN __builtin_COLUMN()
#else
#define KSTD_COLUMN 0
#endif

namespace kstd {
    class SourceLocation final {
        const char* _file;
        const char* _function;
        usize _line;
        usize _column;

    public:
        constexpr SourceLocation(const char* file, const char* function, const usize line, const usize column) noexcept
            : _file(file)
            , _function(function)
            , _line(line)
            , _column(column) {
        }

        KSTD_DEFAULT_MOVE_COPY(SourceLocation, SourceLocation, constexpr)
        ~SourceLocation() noexcept = default;

        static auto current(const char* file = __builtin_FILE(),
                            const char* function = __builtin_FUNCTION(),
                            const usize line = __builtin_LINE(),
                            const usize column = KSTD_COLUMN) noexcept -> SourceLocation {
            return {file, function, line, column};
        }

        [[nodiscard]] auto get_file() const noexcept -> const char* {
            return _file;
        }

        [[nodiscard]] auto get_function() const noexcept -> const char* {
            return _function;
        }

        [[nodiscard]] auto get_line() const noexcept -> usize {
            return _line;
        }

        [[nodiscard]] auto get_column() const noexcept -> usize {
            return _column;
        }
    };
}// namespace kstd

#define KSTD_LOCATION const kstd::SourceLocation& __loc = kstd::SourceLocation::get_current()