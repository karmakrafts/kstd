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

#include "DWARFAttribute.hpp"

#include <libdwarf.h>

#include "DWARFObject.hpp"

namespace kstd {
    static_assert(sizeof(Dwarf_Unsigned) == sizeof(usize));
    static_assert(sizeof(Dwarf_Signed) == sizeof(isize));

    DWARFAttribute::DWARFAttribute(DWARFObject* object, DWARFEntry* entry, Dwarf_Attribute_s* handle) noexcept
        : _object(object)
        , _entry(entry)
        , _handle(handle) {
    }

    DWARFAttribute::~DWARFAttribute() noexcept {
        if(_handle == nullptr) {
            return;
        }
        dwarf_dealloc_attribute(_handle);
    }

    template<>
    auto DWARFAttribute::into<String>() const noexcept -> String {
        char* data = nullptr;
        Dwarf_Error_s* error = nullptr;
        if(dwarf_formstring(_handle, &data, &error) != DW_DLV_OK || data == nullptr) {
            return {};
        }
        String result(data);
        dwarf_dealloc(*_object, data, DW_DLA_STRING);
        return result;
    }

    template<>
    auto DWARFAttribute::into<usize>() const noexcept -> usize {
        usize data = 0;
        Dwarf_Error_s* error = nullptr;
        if(dwarf_formudata(_handle, reinterpret_cast<Dwarf_Unsigned*>(&data), &error) != DW_DLV_OK) {
            return 0;
        }
        return data;
    }

    template<>
    auto DWARFAttribute::into<isize>() const noexcept -> isize {
        isize data = 0;
        Dwarf_Error_s* error = nullptr;
        if(dwarf_formsdata(_handle, reinterpret_cast<Dwarf_Signed*>(&data), &error) != DW_DLV_OK) {
            return 0;
        }
        return data;
    }
}// namespace kstd

#endif// KSTD_PLATFORM_UNIX