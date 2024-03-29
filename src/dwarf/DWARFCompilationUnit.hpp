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

#ifdef KSTD_PLATFORM_UNIX

#include "kstd/Array.hpp"
#include "kstd/Defaults.hpp"
#include "kstd/Queue.hpp"
#include "kstd/Types.hpp"

#include "DWARFEntry.hpp"

struct Dwarf_Die_s;

namespace kstd {
    class DWARFObject;

    class DWARFCompilationUnit {
        friend class DWARFObject;

        DWARFObject* _object;
        Dwarf_Die_s* _handle;
        Array<DWARFEntry> _entries;
        Array<String> _source_files;

    public:
        DWARFCompilationUnit() noexcept;
        DWARFCompilationUnit(DWARFObject* object, Dwarf_Die_s* handle) noexcept;
        KSTD_DEFAULT_MOVE(DWARFCompilationUnit, DWARFCompilationUnit)
        KSTD_NO_COPY(DWARFCompilationUnit, DWARFCompilationUnit)
        ~DWARFCompilationUnit() noexcept;

        [[nodiscard]] operator Dwarf_Die_s*() noexcept {
            return _handle;
        }

        [[nodiscard]] operator const Dwarf_Die_s*() const noexcept {
            return _handle;
        }

        [[nodiscard]] auto handle() noexcept -> Dwarf_Die_s* {
            return _handle;
        }

        [[nodiscard]] auto handle() const noexcept -> const Dwarf_Die_s* {
            return _handle;
        }

        [[nodiscard]] auto object() noexcept -> DWARFObject& {
            return *_object;
        }

        [[nodiscard]] auto object() const noexcept -> const DWARFObject& {
            return *_object;
        }

        [[nodiscard]] auto entries() const noexcept -> const Array<DWARFEntry>& {
            return _entries;
        }

        [[nodiscard]] auto source_files() const noexcept -> const Array<String>& {
            return _source_files;
        }
    };
}// namespace kstd

#endif// KSTD_PLATFORM_UNIX