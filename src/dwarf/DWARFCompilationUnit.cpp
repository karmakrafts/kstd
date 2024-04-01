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

#include "DWARFCompilationUnit.hpp"

#include <dwarf.h>
#include <libdwarf.h>

#include "DWARFObject.hpp"

namespace kstd {
    DWARFCompilationUnit::DWARFCompilationUnit() noexcept
        : _object(nullptr)
        , _handle(nullptr) {
    }

    DWARFCompilationUnit::DWARFCompilationUnit(DWARFObject* object, Dwarf_Die_s* handle) noexcept
        : _object(object)
        , _handle(handle) {
        Queue<Dwarf_Die_s*> queue {};
        queue.push(handle);
        Dwarf_Die_s* child = nullptr;
        Dwarf_Error_s* error;
        while(!queue.empty()) {
            auto* die = queue.front();
            if(dwarf_child(die, &child, &error) != DW_DLV_OK || child == nullptr) {
                continue;
            }
            do {
                _entries.emplace_back(_object, this, child);
                queue.push(child);
            } while(dwarf_siblingof_b(_object->handle(), child, 1, &child, &error) == DW_DLV_OK && child != nullptr);
        }
    }

    DWARFCompilationUnit::~DWARFCompilationUnit() noexcept {
        if(_handle == nullptr) {
            return;
        }
        dwarf_dealloc_die(_handle);
    }
}// namespace kstd

#endif// KSTD_PLATFORM_UNIX