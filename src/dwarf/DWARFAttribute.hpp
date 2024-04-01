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

#include "kstd/Defaults.hpp"
#include "kstd/String.hpp"
#include "kstd/Types.hpp"
#include "kstd/Utility.hpp"

struct Dwarf_Attribute_s;

namespace kstd {
    class DWARFObject;
    class DWARFEntry;

    class DWARFAttribute {
        friend class DWARFEntry;

        DWARFObject* _object;
        DWARFEntry* _entry;
        Dwarf_Attribute_s* _handle;

    public:
        DWARFAttribute() noexcept = default;
        DWARFAttribute(DWARFObject* object, DWARFEntry* entry, Dwarf_Attribute_s* handle) noexcept;
        DWARFAttribute(DWARFAttribute&& other) noexcept;
        KSTD_NO_COPY(DWARFAttribute, DWARFAttribute)
        ~DWARFAttribute() noexcept;

        auto operator=(DWARFAttribute&& other) noexcept -> DWARFAttribute&;

        template<typename T>
        [[nodiscard]] auto into() const noexcept -> T;

        [[nodiscard]] auto object() noexcept -> DWARFObject& {
            return *_object;
        }

        [[nodiscard]] auto object() const noexcept -> const DWARFObject& {
            return *_object;
        }

        [[nodiscard]] auto entry() noexcept -> DWARFEntry& {
            return *_entry;
        }

        [[nodiscard]] auto entry() const noexcept -> const DWARFEntry& {
            return *_entry;
        }

        [[nodiscard]] auto handle() noexcept -> Dwarf_Attribute_s* {
            return _handle;
        }

        [[nodiscard]] auto handle() const noexcept -> const Dwarf_Attribute_s* {
            return _handle;
        }
    };
}// namespace kstd