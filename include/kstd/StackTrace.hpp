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

#include "Array.hpp"
#include "Defaults.hpp"
#include "String.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace kstd {
    class StackTraceElement final {
        void* _address;
        String _binary;
        String _file_name;
        String _function_name;
        usize _line {};
        usize _column {};

    public:
        StackTraceElement() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(StackTraceElement, StackTraceElement)
        ~StackTraceElement() noexcept = default;

        StackTraceElement(void* address, String binary, String file_name, String function_name, const usize line, const usize column) noexcept
            : _address(address)
            , _binary(kstd::move(binary))
            , _file_name(kstd::move(file_name))
            , _function_name(kstd::move(function_name))
            , _line(line)
            , _column(column) {
        }

        [[nodiscard]] auto get_address() const noexcept -> void* {
            return _address;
        }

        [[nodiscard]] auto get_binary() const noexcept -> const String& {
            return _binary;
        }

        [[nodiscard]] auto get_file_name() const noexcept -> const String& {
            return _file_name;
        }

        [[nodiscard]] auto get_function_name() const noexcept -> const String& {
            return _function_name;
        }

        [[nodiscard]] auto get_line() const noexcept -> usize {
            return _line;
        }

        [[nodiscard]] auto get_column() const noexcept -> usize {
            return _column;
        }
    };

    class StackTrace final {
        Array<StackTraceElement> _elements;

        explicit StackTrace(Array<StackTraceElement> elements) noexcept
            : _elements(kstd::move(elements)) {
        }

    public:
        KSTD_DEFAULT_MOVE_COPY(StackTrace, StackTrace)
        ~StackTrace() noexcept = default;

        [[nodiscard]] auto operator [](usize index) const noexcept -> const StackTraceElement& {
            return _elements[index];
        }

        [[nodiscard]] auto get_depth() const noexcept -> usize {
            return _elements.size();
        }

        [[nodiscard]] static auto get_current(usize depth = 32) noexcept -> StackTrace;
    };
}// namespace kstd