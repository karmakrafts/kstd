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
#include "kstd/Path.hpp"
#include "kstd/Types.hpp"
#include "kstd/Utility.hpp"

#include "DWARFCompilationUnit.hpp"

struct Dwarf_Debug_s;

namespace kstd {
    class DWARFObject {
        Path _path;
        Dwarf_Debug_s* _handle;
        Array<DWARFCompilationUnit> _units;

    public:
        explicit DWARFObject(Path path);
        DWARFObject(DWARFObject&& other) noexcept;
        KSTD_NO_COPY(DWARFObject, DWARFObject)
        ~DWARFObject() noexcept;

        auto operator=(DWARFObject&& other) noexcept -> DWARFObject&;

        [[nodiscard]] operator Dwarf_Debug_s*() noexcept {
            return _handle;
        }

        [[nodiscard]] operator const Dwarf_Debug_s*() const noexcept {
            return _handle;
        }

        [[nodiscard]] auto path() const noexcept -> const Path& {
            return _path;
        }

        [[nodiscard]] auto handle() noexcept -> Dwarf_Debug_s* {
            return _handle;
        }

        [[nodiscard]] auto handle() const noexcept -> const Dwarf_Debug_s* {
            return _handle;
        }

        [[nodiscard]] auto units() const noexcept -> const Array<DWARFCompilationUnit>& {
            return _units;
        }
    };
}// namespace kstd

#endif//KSTD_PLATFORM_UNIX