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

/**
 * Implement default move-semantics for the given type.
 * @param n The name of the type to implement default move-semantics.
 * @param t The self type of the type to implement default move-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_DEFAULT_MOVE(n, t, ...)       \
    __VA_ARGS__ n(t&&) noexcept = default; \
    __VA_ARGS__ t& operator=(t&&) noexcept = default;

/**
 * Implement default copy-semantics for the given type.
 * @param n The name of the type to implement default copy-semantics.
 * @param t The self type of the type to implement default copy-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_DEFAULT_COPY(n, t, ...)            \
    __VA_ARGS__ n(const t&) noexcept = default; \
    __VA_ARGS__ t& operator=(const t&) noexcept = default;

/**
 * Implement default copy- and move-semantics for the given type.
 * @param n The name of the type to implement default copy- and move-semantics.
 * @param t The self type of the type to implement default copy- and move-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_DEFAULT_MOVE_COPY(n, t, ...) \
    KSTD_DEFAULT_MOVE(n, t, __VA_ARGS__)  \
    KSTD_DEFAULT_COPY(n, t, __VA_ARGS__)

/**
 * Implement none-move-semantics for the given type.
 * @param n The name of the type to implement none-move-semantics.
 * @param t The self type of the type to implement none-move-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_NO_MOVE(n, t, ...)           \
    __VA_ARGS__ n(t&&) noexcept = delete; \
    __VA_ARGS__ t& operator=(t&&) noexcept = delete;

/**
 * Implement none-copy-semantics for the given type.
 * @param n The name of the type to implement none-copy-semantics.
 * @param t The self type of the type to implement none-copy-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_NO_COPY(n, t, ...)                \
    __VA_ARGS__ n(const t&) noexcept = delete; \
    __VA_ARGS__ t& operator=(const t&) noexcept = delete;

/**
 * Implement none-copy- and none-move-semantics for the given type.
 * @param n The name of the type to implement none-copy- and none-move-semantics.
 * @param t The self type of the type to implement none-copy- and none-move-semantics.
 * @param ... A list of modifiers applied to the constructor and operator implementation
 * this macro expands to.
 */
#define KSTD_NO_MOVE_COPY(n, t, ...) \
    KSTD_NO_MOVE(n, t, __VA_ARGS__)  \
    KSTD_NO_COPY(n, t, __VA_ARGS__)
