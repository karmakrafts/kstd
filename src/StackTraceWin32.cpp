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

#include "kstd/StackTrace.hpp"
#include <DbgEng.h>
#include <DbgHelp.h>

namespace kstd {
    static HMODULE dbgeng = nullptr;
    static IDebugClient* debug_client = nullptr;
    static IDebugSymbols* debug_symbols = nullptr;
    static IDebugControl* debug_control = nullptr;

    void init() {
        dbgeng = LoadLibraryExW(L"dbgeng.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!dbgeng) {
            //TODO: handle error
        }

        const auto debug_create = reinterpret_cast<decltype(&DebugCreate)>(GetProcAddress(dbgeng, "DebugCreate"));
        if (!debug_create) {
            //TODO: handle error
        }

        auto result = debug_create(IID_IDebugClient, reinterpret_cast<void**>(&debug_client));
        if (FAILED(result)) {
            //TODO: handle error
        }

        result = debug_client->QueryInterface(IID_IDebugSymbols, reinterpret_cast<void**>(&debug_symbols));
        if (FAILED(result)) {
            //TODO: handle error
        }

        result = debug_client->QueryInterface(IID_IDebugControl, reinterpret_cast<void**>(&debug_control));
        if (FAILED(result)) {
            //TODO: handle error
        }

        result =debug_client->AttachProcess(0, GetCurrentProcessId(), DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND);
        if (FAILED(result)) {
            //TODO: handle error
        }

        constexpr auto add_options = SYMOPT_CASE_INSENSITIVE | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS
                                                    | SYMOPT_LOAD_LINES | SYMOPT_OMAP_FIND_NEAREST
                                                    | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_AUTO_PUBLICS
                                                    | SYMOPT_NO_PROMPTS;

        constexpr auto remove_options = SYMOPT_NO_CPP | SYMOPT_LOAD_ANYTHING
                                                       | SYMOPT_NO_UNQUALIFIED_LOADS | SYMOPT_EXACT_SYMBOLS
                                                       | SYMOPT_IGNORE_NT_SYMPATH | SYMOPT_PUBLICS_ONLY
                                                       | SYMOPT_NO_PUBLICS | SYMOPT_NO_IMAGE_SEARCH;

        result = debug_symbols->AddSymbolOptions(add_options);
        if (FAILED(result)) {
            //TODO: handle error
        }

        result = debug_symbols->RemoveSymbolOptions(remove_options);
        if (FAILED(result)) {
            //TODO: handle error
        }
    }
}