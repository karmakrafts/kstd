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

#ifdef KSTD_PLATFORM_WINDOWS

#include "kstd/StackTrace.hpp"

#include <DbgEng.h>
#include <DbgHelp.h>

#include "kstd/Atomic.hpp"

namespace kstd {
    static atomic_bool initialized {false};
    static HMODULE dbgeng = nullptr;
    static IDebugClient* debug_client = nullptr;
    static IDebugSymbols* debug_symbols = nullptr;
    static IDebugControl* debug_control = nullptr;

    [[nodiscard]] inline auto ensure_init() -> bool {
        if(initialized) {
            return;
        }

        dbgeng = LoadLibraryExW(L"dbgeng.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if(!dbgeng) {
            return false;
        }

        const auto debug_create = reinterpret_cast<decltype(&DebugCreate)>(GetProcAddress(dbgeng, "DebugCreate"));
        if(!debug_create) {
            return false;
        }

        auto result = debug_create(IID_IDebugClient, reinterpret_cast<void**>(&debug_client));
        if(FAILED(result)) {
            return false;
        }

        result = debug_client->QueryInterface(IID_IDebugSymbols, reinterpret_cast<void**>(&debug_symbols));
        if(FAILED(result)) {
            return false;
        }

        result = debug_client->QueryInterface(IID_IDebugControl, reinterpret_cast<void**>(&debug_control));
        if(FAILED(result)) {
            return false;
        }

        result = debug_client->AttachProcess(0, GetCurrentProcessId(), DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND);
        if(FAILED(result)) {
            return false;
        }

        result = debug_control->WaitForEvent(0, INFINITE);
        if(FAILED(result)) {
            return false;
        }

        constexpr auto add_options = SYMOPT_CASE_INSENSITIVE | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES |
                                     SYMOPT_OMAP_FIND_NEAREST | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_AUTO_PUBLICS | SYMOPT_NO_PROMPTS;

        constexpr auto remove_options = SYMOPT_NO_CPP | SYMOPT_LOAD_ANYTHING | SYMOPT_NO_UNQUALIFIED_LOADS | SYMOPT_EXACT_SYMBOLS |
                                        SYMOPT_IGNORE_NT_SYMPATH | SYMOPT_PUBLICS_ONLY | SYMOPT_NO_PUBLICS | SYMOPT_NO_IMAGE_SEARCH;

        result = debug_symbols->AddSymbolOptions(add_options);
        if(FAILED(result)) {
            return false;
        }

        result = debug_symbols->RemoveSymbolOptions(remove_options);
        if(FAILED(result)) {
            return false;
        }

        return initialized = true;
    }

    auto StackTrace::get_current(const usize depth, const usize skip) noexcept -> StackTrace {
        if(!ensure_init()) {
            return StackTrace {};
        }

        Array<void*> backtrace(depth);
        const auto num_frames = static_cast<usize>(CaptureStackBackTrace(static_cast<ULONG>(skip), static_cast<DWORD>(depth), backtrace.data(), nullptr));

        Array<StackTraceElement> stack_frames(num_frames);
        for(usize i = 0; i < num_frames; i++) {
            const auto address = reinterpret_cast<usize>(backtrace[i]);

            String name;

            ULONG name_size;
            auto result = debug_symbols->GetNameByOffset(address, nullptr, 0, &name_size, nullptr);
            if(SUCCEEDED(result)) {
                name.resize(name_size);

                result = debug_symbols->GetNameByOffset(address, name.data(), name_size, nullptr, nullptr);
                if(FAILED(result)) {
                    name = "";
                }
            }

            String file;
            ULONG file_size;
            ULONG line = 0;

            result = debug_symbols->GetLineByOffset(address, nullptr, nullptr, 0, &file_size, nullptr);
            if(SUCCEEDED(result)) {
                file.resize(file_size);

                result = debug_symbols->GetLineByOffset(address, &line, file.data(), file_size, nullptr, nullptr);
                if(FAILED(result)) {
                    file = "";
                    line = 0;
                }
            }

            String binary;
            String function_name;

            const auto delim_index = name.find_first_of('!');
            if(delim_index == String::npos) {
                function_name = kstd::move(name);
            }
            else {
                binary = name.substr(0, delim_index);
                function_name = name.substr(delim_index + 1);
            }

            stack_frames[i] = StackTraceElement {backtrace[i], binary, file, function_name, line, 0};
        }

        return StackTrace {kstd::move(stack_frames)};
    }

}// namespace kstd

#endif