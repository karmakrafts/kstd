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

#include <mimalloc.h>

#include "Defaults.hpp"
#include "FixedArray.hpp"

namespace kstd {
    /**
     * A default allocator implementation which uses the mimalloc memory allocator.
     *
     * @tparam T The type to be allocated by this allocator type.
     */
    template<typename T>
    struct Allocator final {
        using value_type = T;

    private:
        static constexpr bool is_void = is_same<T, void>;
        static constexpr usize size = is_void ? 1 : sizeof(T);
        static constexpr usize alignment = is_void ? alignof(void*) : alignof(T);

    public:
        Allocator() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(Allocator, Allocator)
        ~Allocator() noexcept = default;

        [[nodiscard]] auto allocate(const usize count) noexcept -> T* {
            return static_cast<T*>(::mi_malloc_aligned(size * count, alignment));
        }

        [[nodiscard]] auto reallocate(T* value, [[maybe_unused]] const usize old_count, const usize count) noexcept -> T* {
            return static_cast<T*>(::mi_realloc_aligned(value, size * count, alignment));
        }

        auto free(T* memory) noexcept -> void {
            if(memory == nullptr) {
                return;
            }
            ::mi_free(memory);
        }
    };

    namespace concepts {
        /*
         * In order to define a custom allocator, the following API must be implemented on your allocator type A:
         *	- typename A::value_type					    : the type of the underlying element being allocated
         *	- A().allocate(usize)				            : A function for allocating new objects and leaving them uninitialized
         *	- A().reallocate(A::value_type*, usize, usize)  : A function for reallocating memory blocks and leaving them uninitialized
         *	- A().free(A::value_type*)					    : A function for deallocating objects
         * When the type specified in the allocator is void, the size of each element shall be 1 and it should have the default alignment.
         */
        template<typename T>
        concept Allocator = requires(T value, usize size) {
            typename T::value_type;
            requires is_convertible<typename T::value_type*, decltype(value.allocate(size))>;
            requires is_convertible<typename T::value_type*, decltype(value.reallocate(nullptr, size, size))>;
            requires is_same<void, decltype(value.free(nullptr))>;
        };
    }// namespace concepts
}// namespace kstd