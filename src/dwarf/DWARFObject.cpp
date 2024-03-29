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

#include "DWARFObject.hpp"

#include <dwarf.h>
#include <libdwarf.h>

namespace kstd {
    DWARFObject::DWARFObject(Path path)
        : _path(kstd::move(path))
        , _handle(nullptr) {
        Dwarf_Error_s* error;
        if(dwarf_init_path_a(_path.string().c_str(), nullptr, 0, DW_GROUPNUMBER_ANY, 0, nullptr, nullptr, &_handle, &error) != DW_DLV_OK ||
           _handle == nullptr) {
            throw;
        }
        Dwarf_Unsigned cu_header_length;
        Dwarf_Half cu_header_version;
        Dwarf_Off cu_abbrev_offset;
        Dwarf_Half cu_address_size;
        Dwarf_Unsigned cu_next_header_offset;
        Dwarf_Half cu_length_size;
        Dwarf_Half cu_extension_size;
        Dwarf_Sig8 cu_type_signature;
        Dwarf_Unsigned cu_type_offset;
        Dwarf_Half cu_header_type;
        while(dwarf_next_cu_header_d(_handle, TRUE, &cu_header_length, &cu_header_version, &cu_abbrev_offset, &cu_address_size,
                                     &cu_length_size, &cu_extension_size, &cu_type_signature, &cu_type_offset, &cu_next_header_offset,
                                     &cu_header_type, &error) == DW_DLV_OK) {
            Dwarf_Die_s* unit = nullptr;
            if(dwarf_siblingof_b(_handle, nullptr, TRUE, &unit, &error) != DW_DLV_OK || unit == nullptr) {
                continue;
            }
            _units.emplace_back(this, unit);
        }
    }

    DWARFObject::DWARFObject(DWARFObject&& other) noexcept
        : _path(kstd::move(other._path))
        , _handle(other._handle) {
        other._handle = nullptr;
    }

    DWARFObject::~DWARFObject() noexcept {
        if(_handle == nullptr) {
            return;
        }
        dwarf_finish(_handle);
    }

    auto DWARFObject::operator=(DWARFObject&& other) noexcept -> DWARFObject& {
        _path = move(other._path);
        _handle = other._handle;
        _units = move(other._units);
        other._handle = nullptr;
        return *this;
    }
}// namespace kstd

#endif// KSTD_PLATFORM_UNIX