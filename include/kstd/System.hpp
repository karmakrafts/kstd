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

// NOLINTBEGIN
#ifdef KSTD_PLATFORM_MACOS
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <string.h>
#include <wchar.h>
// NOLINTEND

#include "Concepts.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#ifdef KSTD_COMPILER_MSVC
#define KSTD_ALLOCA(s) _alloca(s)
#else
#define KSTD_ALLOCA(s) __builtin_alloca(s)
#endif

namespace kstd::system {
    namespace iob {
        // NOLINTBEGIN
#if defined(KSTD_PLATFORM_LINUX)
        inline auto out = stdout;
        inline auto err = stderr;
        inline auto in = stdin;
#elif defined(KSTD_PLATFORM_WINDOWS)
        inline auto out = ::__acrt_iob_func(1);
        inline auto err = ::__acrt_iob_func(2);
        inline auto in = ::__acrt_iob_func(0);
#else
#if defined(__DARWIN_UNIX03)
        inline auto out = ::__stdoutp;
        inline auto err = ::__stderrp;
        inline auto in = ::__stdinp;
#else
        inline auto out = &::__sF[1];
        inline auto err = &::__sF[2];
        inline auto in = &::__sF[0];
#endif
#endif
        // NOLINTEND
    }// namespace iob

    using File = ::FILE;

    [[noreturn]] inline void exit(const i32 code) noexcept {
        ::exit(code);
    }

    template<typename... ARGS>
    i32 scanf(const char* fmt, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::scanf(fmt, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 fprintf(File* file, const char* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::fprintf(file, format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 printf(const char* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::printf(format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 sprintf(char* buffer, const char* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::sprintf(buffer, format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 swprintf(const wchar_t* buffer, const wchar_t* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::swprintf(buffer, format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 fwprintf(File* file, const wchar_t* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::fwprintf(file, format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 wprintf(const wchar_t* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 2 parameters!");
        return ::wprintf(format, forward<ARGS>(args)...);// NOLINT
    }

    template<typename... ARGS>
    i32 wscanf(wchar_t* buffer, const wchar_t* format, ARGS&&... args) noexcept {
        static_assert(sizeof...(ARGS) >= 1, "Function requires at least 3 parameters!");
        return ::wscanf(buffer, format, forward<ARGS>(args)...);// NOLINT
    }

    // Strings
    template<concepts::Integer T>
    [[nodiscard]] usize get_string_length(const T* str) noexcept {
        if constexpr(is_same<T, char>) {
            return ::strlen(str);
        }
        else if constexpr(is_same<T, wchar_t>) {
            return ::wcslen(str);
        }
        else {
            auto* ptr = str;
            while(*ptr != static_cast<T>('\0')) {
                ++ptr;
            }
            return static_cast<usize>(ptr - str);// Use pointer difference
        }
    }

    template<concepts::Integer T>
    void copy_string(T* dst, const T* src) noexcept {
        if constexpr(is_same<T, char>) {
            ::strcpy(dst, src);
        }
        else if constexpr(is_same<T, wchar_t>) {
            ::wcscpy(dst, src);
        }
        else {
            const auto dst_length = get_string_length(dst);
            const auto src_length = get_string_length(src);
            const auto length = min(dst_length, src_length);
            ::memcpy(dst, src, (length + 1) * sizeof(T));
        }
    }

    template<concepts::Integer T>
    [[nodiscard]] i32 compare_string(const T* lhs, const T* rhs) noexcept {
        if constexpr(is_same<T, char>) {
            return ::strcmp(lhs, rhs);
        }
        else if constexpr(is_same<T, wchar_t>) {
            return ::wcscmp(lhs, rhs);
        }
        else {
            const auto a_length = get_string_length(lhs);
            const auto b_length = get_string_length(rhs);

            if(a_length != b_length) {
                return static_cast<i32>(b_length - a_length);// Narrowing conversion -.-
            }

            return ::memcmp(lhs, rhs, a_length);
        }
    }

    template<concepts::Integer T>
    T* concat_string(T* dst, const T* src) noexcept {
        if constexpr(is_same<T, char>) {
            return ::strcat(dst, src);
        }
        else if constexpr(is_same<T, wchar_t>) {
            return ::wcscat(dst, src);
        }
        else {
            ::memcpy(dst + get_string_length(dst), src, get_string_length(src) + 1);
            return dst;
        }
    }
}// namespace kstd::system