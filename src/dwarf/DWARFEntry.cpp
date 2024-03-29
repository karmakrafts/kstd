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

#ifdef KSTD_PLATFORM_UNIX

#include "DWARFEntry.hpp"

#include <dwarf.h>
#include <libdwarf.h>

#include "DWARFObject.hpp"

namespace kstd {
    DWARFEntry::DWARFEntry() noexcept
        : _object(nullptr)
        , _unit(nullptr)
        , _handle(nullptr) {
    }

    DWARFEntry::DWARFEntry(DWARFObject* object, DWARFCompilationUnit* unit, Dwarf_Die_s* handle) noexcept
        : _object(object)
        , _unit(unit)
        , _handle(handle) {
        Dwarf_Error_s* error;
        Dwarf_Attribute_s** attribs = nullptr;
        Dwarf_Signed num_attribs = 0;
        if(dwarf_attrlist(handle, &attribs, &num_attribs, &error) != DW_DLV_OK || attribs == nullptr) {
            return;
        }
        dwarf_dealloc(object->handle(), attribs, DW_DLA_LIST);
    }

    DWARFEntry::~DWARFEntry() noexcept {
        if(_handle == nullptr) {
            return;
        }
        dwarf_dealloc_die(_handle);
    }
}// namespace kstd

#endif// KSTD_PLATFORM_UNIX