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
#include "Utility.hpp"

/*
 * In order to define a custom allocator, the following API must be implemented on your allocator type A:
 *	- typename A::type					     : the type of the underlying element being allocated
 *	- bool A::has_state						 : true if the allocator instance carries state which needs to be retained
 *	- A().allocate(usize)				     : A function for allocating new objects and leaving them uninitialized
 *	- A().reallocate(A::Type*, usize, usize) : A function for reallocating memory blocks and leaving them uninitialized
 *	- A().free(A::Type*)					 : A function for deallocating objects
 */
namespace kstd {
    /**
     * A default allocator implementation which uses the mimalloc memory allocator.
     *
     * @tparam T The type to be allocated by this allocator type.
     */
    template<typename T>
    struct Allocator final {
        using value_type = T;
        static constexpr bool has_state = false;

    private:
        using self_type = Allocator<value_type>;

    public:
        Allocator() noexcept = default;
        KSTD_DEFAULT_MOVE_COPY(Allocator, self_type)
        ~Allocator() noexcept = default;

        [[nodiscard]] auto allocate(const usize count) noexcept -> value_type* {
            return static_cast<value_type*>(::mi_malloc_aligned(sizeof(value_type) * count, alignof(value_type)));
        }

        [[nodiscard]] auto reallocate(value_type* value, [[maybe_unused]] const usize old_count, const usize count) noexcept
            -> value_type* {
            return static_cast<value_type*>(::mi_realloc_aligned(value, sizeof(value_type) * count, alignof(value_type)));
        }

        auto free(value_type* memory) noexcept -> void {
            if(memory == nullptr) {
                return;
            }
            ::mi_free(memory);
        }
    };
}// namespace kstd