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

#include "kstd/StackTrace.hpp"

#include <cxxabi.h>
#include <dlfcn.h>
#include <dwarf.h>
#include <libdwarf.h>
#include <libunwind.h>

#include "kstd/Array.hpp"
#include "kstd/FixedArray.hpp"
#include "kstd/Functional.hpp"
#include "kstd/Math.hpp"
#include "kstd/Queue.hpp"
#include "kstd/Tuple.hpp"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

namespace kstd {
    // Make sure the pointer size matches the unwind word size
    static_assert(sizeof(unw_word_t) >= sizeof(void*));

    constexpr usize MAX_NAME_SIZE = 4096;

    using DWARFDie = Dwarf_Die_s;
    using DWARFObject = Dwarf_Debug_s;
    using DWARFAttribute = Dwarf_Attribute_s;
    using DWARFError = Dwarf_Error_s;
    using DWARFHalf = Dwarf_Half;
    using DWARFSigned = Dwarf_Signed;
    using DWARFUnsigned = Dwarf_Unsigned;
    using DWARFBool = Dwarf_Bool;
    using DWARFOff = Dwarf_Off;
    using DWARFSig8 = Dwarf_Sig8;

    [[nodiscard]] inline auto demangle(const String& name) noexcept -> String {
        int status;
        auto* memory = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
        if(status != 0 || memory == nullptr) {
            return name;
        }
        String result(memory);
        free(memory);// This is heap memory allocated by __cxa_demangle when the output buffer is specified as nullptr
        return result;
    }

    [[nodiscard]] inline auto get_binary(const void* address) noexcept -> String {
        Dl_info symbol_info;
        if(dladdr(address, &symbol_info) > 0 && symbol_info.dli_fname) {
            return {symbol_info.dli_fname};
        }
        return "";
    }

    [[nodiscard]] inline auto get_attrib(DWARFDie* die, const u16 attribute) noexcept -> DWARFAttribute* {
        DWARFBool has_attrib = 0;
        DWARFError* error = nullptr;
        if(dwarf_hasattr(die, attribute, &has_attrib, &error) != DW_DLV_OK || !has_attrib) {
            return nullptr;
        }
        DWARFAttribute* attrib = nullptr;
        if(dwarf_attr(die, attribute, &attrib, &error) != DW_DLV_OK) {
            return nullptr;
        }
        return attrib;
    }

    template<typename T, decltype(auto) TFormer, DWARFHalf... TForms>
    requires(is_callable<decltype(TFormer), DWARFAttribute*, T*, DWARFError**>)
    [[nodiscard]] auto get_attrib_value(DWARFObject* object, DWARFDie* die, const DWARFHalf attribute) noexcept
        -> Pair<DWARFAttribute*, T> {
        auto* attrib = get_attrib(die, attribute);
        if(attrib == nullptr) {
            return {nullptr, {}};
        }
        DWARFError* error = nullptr;
        DWARFHalf form;
        if(dwarf_whatform(attrib, &form, &error) != DW_DLV_OK || ((form != TForms) && ...)) {
            dwarf_dealloc(object, attrib, DW_DLA_ATTR);
            return {nullptr, {}};
        }
        T value {};
        if(TFormer(attrib, &value, &error) != DW_DLV_OK) {
            dwarf_dealloc(object, attrib, DW_DLA_ATTR);
            return {nullptr, {}};
        }
        return {attrib, value};
    }

    inline auto extract_die_information(DWARFObject* object,
                                        DWARFDie* die,
                                        String& file_name,
                                        usize& line,
                                        usize& column,
                                        const Array<String>& file_names) noexcept -> void {
        // Extract source file name
        const auto [file_attrib, file_index] =
            get_attrib_value<DWARFUnsigned, &dwarf_formudata, DW_FORM_udata, DW_FORM_sdata, DW_FORM_implicit_const, DW_FORM_data1,
                             DW_FORM_data2, DW_FORM_data4, DW_FORM_data8, DW_FORM_data16>(object, die, DW_AT_decl_file);
        if(file_attrib != nullptr) {
            file_name = file_names[file_index];
            dwarf_dealloc(object, file_attrib, DW_DLA_ATTR);
        }
        // Extract line number
        const auto [line_attrib, line_no] =
            get_attrib_value<DWARFUnsigned, &dwarf_formudata, DW_FORM_udata, DW_FORM_sdata, DW_FORM_implicit_const, DW_FORM_data1,
                             DW_FORM_data2, DW_FORM_data4, DW_FORM_data8, DW_FORM_data16>(object, die, DW_AT_decl_line);
        if(line_attrib != nullptr) {
            line = line_no;
            dwarf_dealloc(object, line_attrib, DW_DLA_ATTR);
        }
        // Extract column number
        const auto [column_attrib, column_no] =
            get_attrib_value<DWARFUnsigned, &dwarf_formudata, DW_FORM_udata, DW_FORM_sdata, DW_FORM_implicit_const, DW_FORM_data1,
                             DW_FORM_data2, DW_FORM_data4, DW_FORM_data8, DW_FORM_data16>(object, die, DW_AT_decl_column);
        if(column_attrib != nullptr) {
            column = column_no;
            dwarf_dealloc(object, column_attrib, DW_DLA_ATTR);
        }
    }

    [[nodiscard]] inline auto process_die(DWARFObject* object,
                                          DWARFDie* die,
                                          const String& mangled_name,
                                          String& file_name,
                                          usize& line,
                                          usize& column,
                                          Queue<DWARFDie*>& queue,
                                          const Array<String>& file_names) noexcept -> bool {// NOLINT
        // Check if our current DIE is a subprogram aka. function
        DWARFHalf tag;
        DWARFError* error = nullptr;
        if(dwarf_tag(die, &tag, &error) == DW_DLV_OK && tag == DW_TAG_subprogram) {
            char* name;
            if(dwarf_diename(die, &name, &error) != DW_DLV_OK || name == nullptr) {
                return false;
            }
            // If the DIE name matches the mangled name, this is an unmangled exported function
            if(mangled_name == name) {
                extract_die_information(object, die, file_name, line, column, file_names);
                dwarf_dealloc(object, name, DW_DLA_STRING);// Deallocate name string when we're done
                return true;                               // Probably an unmangled libc function
            }
            dwarf_dealloc(object, name, DW_DLA_STRING);// Deallocate name string when we're done
            // If the linkage name matches the mangled name, this is a mangled (likely C++) function and we found it
            const auto [linkage_name_attrib, linkage_name] =
                get_attrib_value<char*, &dwarf_formstring, DW_FORM_strp, DW_FORM_string, DW_FORM_strx, DW_FORM_strx1, DW_FORM_strx2,
                                 DW_FORM_strx3, DW_FORM_strx4>(object, die, DW_AT_linkage_name);
            if(linkage_name_attrib != nullptr && linkage_name != nullptr && mangled_name == linkage_name) {
                extract_die_information(object, die, file_name, line, column, file_names);
                dwarf_dealloc(object, linkage_name, DW_DLA_STRING);
                dwarf_dealloc(object, linkage_name_attrib, DW_DLA_ATTR);
                return true;
            }
            return false;
        }
        // Check if we have any children and if we do, add them to the queue before we return
        DWARFDie* child_die = nullptr;
        if(dwarf_child(die, &child_die, &error) == DW_DLV_OK && child_die != nullptr) {
            do {
                queue.push(child_die);
            } while(dwarf_siblingof_b(object, child_die, TRUE, &child_die, &error) == DW_DLV_OK && child_die != nullptr);
        }
        return false;
    }

    [[nodiscard]] inline auto get_source_info(const String& binary, const String& mangled_name) noexcept -> Tuple<String, usize, usize> {
        // Initialize DWARF debug object
        DWARFObject* object = nullptr;
        DWARFError* error = nullptr;
        if(dwarf_init_path_a(binary.c_str(), nullptr, 0, DW_GROUPNUMBER_ANY, 0, nullptr, nullptr, &object, &error) != DW_DLV_OK) {
            return {"", 0, 0};
        }
        // Iterate through compilation units
        DWARFUnsigned cu_header_length;
        DWARFHalf cu_header_version;
        DWARFOff cu_abbrev_offset;
        DWARFHalf cu_address_size;
        DWARFUnsigned cu_next_header_offset;
        DWARFHalf cu_length_size;
        DWARFHalf cu_extension_size;
        DWARFSig8 cu_type_signature;
        DWARFUnsigned cu_type_offset;
        DWARFHalf cu_header_type;
        String file_name {};
        usize line = 0;
        usize column = 0;
        bool found = false;
        while(dwarf_next_cu_header_d(object, TRUE, &cu_header_length, &cu_header_version, &cu_abbrev_offset, &cu_address_size,
                                     &cu_length_size, &cu_extension_size, &cu_type_signature, &cu_type_offset, &cu_next_header_offset,
                                     &cu_header_type, &error) == DW_DLV_OK) {
            // Retrieve compilation unit root DIE
            DWARFDie* die;
            if(dwarf_siblingof_b(object, nullptr, TRUE, &die, &error) != DW_DLV_OK || die == nullptr) {
                continue;
            }
            // Extract source file name table
            Array<String> source_files {};
            char** file_names = nullptr;
            DWARFSigned num_file_names = 0;
            if(dwarf_srcfiles(die, &file_names, &num_file_names, &error) == DW_DLV_OK && file_names != nullptr) {
                source_files.reserve(num_file_names);
                for(usize i = 0; i < num_file_names; ++i) {
                    source_files.emplace_back(file_names[i]);
                }
                dwarf_dealloc(object, file_names, DW_DLA_LIST);
            }
            // Use heap recursion to find our desired function
            Queue<DWARFDie*> queue {};
            queue.push(die);
            while(!queue.empty()) {
                auto* current_die = queue.front();
                if(process_die(object, current_die, mangled_name, file_name, line, column, queue, source_files)) {
                    // Make sure we free all remaining DIEs
                    while(!queue.empty()) {
                        dwarf_dealloc_die(queue.front());
                        queue.pop();
                    }
                    found = true;
                    break;
                }
                dwarf_dealloc_die(current_die);// Deallocate die after we're done
                queue.pop();
            }
            if(found) {
                break;
            }
        }

        dwarf_finish(object);
        return {kstd::move(file_name), line, column};
    }

    auto StackTrace::get_current(const usize depth, const usize skip) noexcept -> StackTrace {
        if(depth == 0) {
            return {};
        }
        unw_context_t context;
        if(unw_getcontext(&context) != UNW_ESUCCESS) {
            return {};
        }

        unw_cursor_t cursor;
        if(unw_init_local(&cursor, &context) != UNW_ESUCCESS) {
            return {};
        }

        FixedArray<char, MAX_NAME_SIZE> name_buffer {};
        Array<StackTraceElement> stack_frames {};
        usize index = 0;

        while(unw_step(&cursor) > 0) {
            if(index == depth - 1) {
                break;// If we reached the maximum specified depth, stop
            }
            if(index < static_cast<usize>(min<isize>(0, static_cast<isize>(skip) - 1))) {
                ++index;
                continue;
            }
            void* ip;
            if(unw_get_reg(&cursor, UNW_REG_IP, reinterpret_cast<unw_word_t*>(&ip)) != UNW_ESUCCESS || ip == nullptr) {
                stack_frames.emplace_back();
                ++index;
                continue;
            }
            unw_word_t offset;
            if(unw_get_proc_name(&cursor, name_buffer.data(), MAX_NAME_SIZE, &offset) != UNW_ESUCCESS) {
                stack_frames.emplace_back();
                ++index;
                continue;
            }

            const String mangled_name(name_buffer.data());
            auto function_name = demangle(mangled_name);
            auto binary = get_binary(ip);
            auto source_info = get_source_info(binary, mangled_name);
            stack_frames.emplace_back(ip, kstd::move(binary), get<0>(source_info), kstd::move(function_name), get<1>(source_info),
                                      get<2>(source_info));
            ++index;
        }

        return {move(stack_frames)};
    }
}// namespace kstd

#endif//KSTD_PLATFORM_UNIX