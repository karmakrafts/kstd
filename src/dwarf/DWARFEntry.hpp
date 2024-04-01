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
#include "kstd/Utility.hpp"

#include "DWARFAttribute.hpp"

struct Dwarf_Die_s;

namespace kstd {
    class DWARFObject;
    class DWARFCompilationUnit;

    class DWARFEntry {
        friend class DWARFCompilationUnit;

        DWARFObject* _object;
        DWARFCompilationUnit* _unit;
        Dwarf_Die_s* _handle;
        Array<DWARFAttribute> _attributes;

    public:
        DWARFEntry() noexcept;
        DWARFEntry(DWARFObject* object, DWARFCompilationUnit* unit, Dwarf_Die_s* handle) noexcept;
        DWARFEntry(DWARFEntry&& other) noexcept;
        KSTD_NO_COPY(DWARFEntry, DWARFEntry)
        ~DWARFEntry() noexcept;

        auto operator=(DWARFEntry&& other) noexcept -> DWARFEntry&;

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

        [[nodiscard]] auto unit() noexcept -> DWARFCompilationUnit& {
            return *_unit;
        }

        [[nodiscard]] auto unit() const noexcept -> const DWARFCompilationUnit& {
            return *_unit;
        }

        [[nodiscard]] auto attributes() const noexcept -> const Array<DWARFAttribute>& {
            return _attributes;
        }
    };
}// namespace kstd

#endif//KSTD_PLATFORM_UNIX
