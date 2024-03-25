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
#include "Utility.hpp"

namespace kstd {
    template<typename T, concepts::Allocator TAllocator = Allocator<T>>
    struct Array final {
        using element_type = T;
        using allocator_type = TAllocator;

    private:
        using self_type = Array<T, TAllocator>;

        TAllocator _allocator;
        usize _size;
        usize _capacity;
        T* _data;

    public:
        Array() noexcept
            : _allocator()
            , _size(0)
            , _capacity(1)
            , _data(_allocator.allocate(_capacity)) {
            new(_data) T();
        }

        Array(const self_type& other) noexcept
            : _allocator()
            , _size(other._size)
            , _capacity(other._capacity)
            , _data(_allocator.allocate(_capacity)) {
            memcpy(_data, other._data, _size * sizeof(T));
        }

        Array(self_type&& other) noexcept
            : _allocator(move(other._allocator))
            , _size(other._size)
            , _capacity(other._capacity)
            , _data(other._data) {
            other._data = nullptr;
        }

        ~Array() noexcept {
            if(_data != nullptr) {
                _allocator.free(_data);
            }
        }

        auto operator=(const self_type& other) noexcept -> self_type& {
            if(&other == this) {
                return *this;
            }
            const auto size = other._size;
            resize(size);
            memcpy(_data, other._data, size * sizeof(T));
            return *this;
        }

        auto operator=(self_type&& other) noexcept -> self_type& {
            _allocator = move(other._allocator);
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            other._data = nullptr;
            return *this;
        }

        auto resize(const usize count) noexcept -> void {
            if(count == _capacity) {
                return;
            }
            // Copy old elements and discard any elements we don't need
            const auto retained_count = min(_size, count);
            const auto discarded_count = _size - retained_count;
            auto* old_elements = _allocator.allocate(retained_count);
            memcpy(old_elements, _data, retained_count * sizeof(T));
            for(usize i = 0; i < discarded_count; ++i) {
                // Call the destructor of every element that gets cut off
                _data[count + i].~T();
            }
            // Re-allocate the memory
            _data = _allocator.reallocate(_data, _capacity, count);
            // Move-reconstruct the old elements
            for(usize i = 0; i < _size; ++i) {
                new(&_data[i]) T(kstd::move(old_elements[i]));
            }
            // Default init new elements
            const auto num_new_elements = count - _capacity;
            for(usize i = 0; i < num_new_elements; ++i) {
                new(&_data[_capacity + i]) T();// Default initialize newly added elements
            }
            _allocator.free(old_elements);
            _capacity = count;
        }

        auto reserve(const usize count) noexcept -> void {
            if(count <= _capacity) {
                return;
            }
            resize(count);
        }

        auto push_back(T value) noexcept -> void {
            reserve(_size + 1);
            _data[_size++] = move(value);
        }

        template<typename... TArgs>
        auto emplace_back(TArgs&&... args) noexcept -> void {
            reserve(_size + 1);
            new(&_data[_size++]) T(kstd::forward<TArgs>(args)...);
        }

        [[nodiscard]] auto get_size() const noexcept -> usize {
            return _size;
        }

        [[nodiscard]] auto operator[](const usize index) noexcept -> T& {
            return _data[index];
        }

        [[nodiscard]] auto operator[](const usize index) const noexcept -> const T& {
            return _data[index];
        }
    };
}// namespace kstd