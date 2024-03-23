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

#include "Allocator.hpp"
#include "Defaults.hpp"
#include "Types.hpp"

namespace kstd {
    template<typename T, usize TBufferSize>
    struct StackAllocator final {
        using value_type = T;
        using fallback_allocator_type = Allocator<T>;

        static constexpr bool has_state = true;
        static constexpr usize buffer_size = TBufferSize;

    private:
        FixedArray<value_type, TBufferSize> _buffer;
        u8* _head;

        [[nodiscard]] auto is_address_in_range(const void* address) const noexcept -> bool {
            const auto* byte_address = static_cast<const u8*>(address);
            const auto* base_address = reinterpret_cast<const u8*>(_buffer.get_const_data());
            const auto* last_address = base_address + _buffer.get_size() * sizeof(value_type);
            return byte_address >= base_address && byte_address <= last_address;
        }

    public:
        StackAllocator() noexcept
            : _head(_buffer.get_data()) {
        }

        KSTD_DEFAULT_MOVE_COPY(StackAllocator, StackAllocator)
        ~StackAllocator() noexcept = default;

        [[nodiscard]] auto allocate(const usize count) noexcept -> value_type* {
            const auto offset = reinterpret_cast<usize>(_head) - reinterpret_cast<usize>(_buffer.get_const_data());
            if(offset == _buffer.get_size()) {
                return fallback_allocator_type().allocate(count);
            }
            auto* memory = _head;
            _head += count * sizeof(value_type);
            *reinterpret_cast<usize*>(memory) = count;
            return reinterpret_cast<value_type*>(memory);
        }

        [[nodiscard]] auto reallocate(value_type* memory, const usize old_count, const usize count) noexcept -> value_type* {
            if(!is_address_in_range(memory)) {
                return fallback_allocator_type().reallocate(memory, old_count, count);
            }
            if(count <= old_count) {
                return memory;
            }
            const auto old_size = old_count * sizeof(value_type);
            if(auto* old_address = _head - old_size; reinterpret_cast<u8*>(memory) == old_address) {
                const auto offset = (count * sizeof(value_type)) - old_size;
                const auto new_size = reinterpret_cast<usize>(_head + offset) - reinterpret_cast<usize>(_buffer.get_const_data());
                const auto new_count = new_size / sizeof(value_type);
                if(new_count <= _buffer.get_size()) {
                    _head += offset;
                    return old_address;
                }
            }
            free(memory);
            return allocate(count);
        }

        auto free(value_type* memory) noexcept -> void {
            if(!is_address_in_range(memory)) {
                fallback_allocator_type().free(memory);
            }
        }
    };
}// namespace kstd