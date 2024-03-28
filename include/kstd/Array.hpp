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
#include "Concepts.hpp"
#include "Panic.hpp"
#include "Slice.hpp"
#include "Utility.hpp"

namespace kstd {
    template<typename T, concepts::Allocator<T> TAllocator = Allocator<T>>
    struct Array final {
        using element_type = T;
        using allocator_type = TAllocator;
        using iterator = T*;
        using const_iterator = const T*;
        using slice_type = Slice<T>;

    private:
        using self_type = Array<T, TAllocator>;

        TAllocator _allocator;
        usize _size;
        usize _capacity;
        T* _data;

        auto resize_internal(const usize count) noexcept -> void {
            // Copy old elements and discard any elements we don't need
            const auto retained_count = min(_size, count);
            const auto discarded_count = _size - retained_count;
            for(usize i = 0; i < discarded_count; ++i) {
                // Call the destructor of every element that gets cut off
                _data[count + i].~T();
            }
            // Re-allocate the memory
            auto* new_data = _allocator.allocate(count);
            // Move-reconstruct the old elements
            for(usize i = 0; i < _size; ++i) {
                new(&new_data[i]) T(kstd::move(_data[i]));
            }
            _allocator.free(_data);// Free old data
            _data = new_data;      // Swap data pointers
            _capacity = count;
        }

        template<typename THead, typename... TTail>
        constexpr auto set_all_impl(const usize offset, THead&& head, TTail&&... tail) noexcept -> void {
            _data[offset] = head;
            if constexpr(sizeof...(TTail) > 0) {
                set_all_impl<TTail...>(offset + 1, forward<TTail>(tail)...);
            }
        }

    public:
        Array() noexcept
            : _allocator()
            , _size(0)
            , _capacity(1)
            , _data(_allocator.allocate(_capacity)) {
            new(_data) T();
        }

        Array(const usize size, const T& default_value = {}) noexcept
            : _allocator()
            , _size(size)
            , _capacity(_size)
            , _data(_allocator.allocate(_capacity)) {
            for(usize i = 0; i < size; ++i) {
                new(&_data[i]) T(default_value);
            }
        }

        Array(const self_type& other) noexcept
            : _allocator()
            , _size(other._size)
            , _capacity(other._capacity)
            , _data(_allocator.allocate(_capacity)) {
            memcpy(_data, other._data, _size * sizeof(T));
        }

        Array(self_type&& other) noexcept
            : _allocator(other._allocator)
            , _size(other._size)
            , _capacity(other._capacity)
            , _data(_allocator.allocate(_capacity)) {
            for(usize i = 0; i < _capacity; ++i) {
                new(&_data[i]) T(kstd::move(other._data[i]));
            }
        }

        Array(const slice_type& slice) noexcept
            : _allocator()
            , _size(slice.size())
            , _capacity(_size)
            , _data(_allocator.allocate(_capacity)) {
            slice.copy_to(_data, _size);
        }

        ~Array() noexcept {
            if(_data == nullptr) {
                return;
            }
            for(usize i = 0; i < _size; ++i) {
                _data[i].~T();// Call destructor on all contained elements
            }
            _allocator.free(_data);
        }

        [[nodiscard]] operator slice_type() const noexcept {
            return {_data, _size};
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
            _allocator = other._allocator;
            resize(other._size);
            for(usize i = 0; i < _capacity; ++i) {
                new(&_data[i]) T(kstd::move(other._data[i]));
            }
            return *this;
        }

        template<concepts::AssignableAs<T>... TArgs>
        auto set_all(const usize index, TArgs&&... values) noexcept -> void {
            set_all_impl<TArgs...>(index, kstd::forward<TArgs>(values)...);
        }

        auto insert(const usize index, const T& value) noexcept -> void {
            reserve(_size + 1);

            for(isize i = _size - 1; i >= static_cast<isize>(index); --i) {
                new(&_data[i + 1]) T(kstd::move(_data[i]));
            }

            _data[index] = value;
            ++_size;
        }

        auto insert(const usize index, T&& value) noexcept -> void {
            reserve(_size + 1);

            for(isize i = _size - 1; i >= static_cast<isize>(index); --i) {
                new(&_data[i + 1]) T(kstd::move(_data[i]));
            }

            _data[index] = value;
            ++_size;
        }

        template<concepts::AssignableAs<T>... TArgs>
        auto insert_all(const usize index, TArgs&&... args) noexcept -> void {
            reserve(_size + sizeof...(TArgs));

            for(isize i = _size - 1; i >= static_cast<isize>(index); --i) {
                new(&_data[i + sizeof...(TArgs)]) T(kstd::move(_data[i]));
            }

            set_all_impl(index, kstd::forward<TArgs>(args)...);
            _size += sizeof...(TArgs);
        }

        [[nodiscard]] auto begin() noexcept -> iterator {
            return _data;
        }

        [[nodiscard]] auto end() noexcept -> iterator {
            return _data + _size;
        }

        [[nodiscard]] auto cbegin() const noexcept -> const_iterator {
            return _data;
        }

        [[nodiscard]] auto cend() const noexcept -> const_iterator {
            return _data + _size;
        }

        auto resize(const usize size) noexcept -> void {
            resize_internal(size);
            // Default init new elements
            const auto num_new_elements = size - _capacity;
            for(usize i = 0; i < num_new_elements; ++i) {
                new(&_data[_capacity + i]) T();// Default initialize newly added elements
            }
            _size = size;
        }

        auto reserve(const usize size) noexcept -> void {
            if(size <= _capacity) {
                return;
            }
            resize_internal(size);
        }

        auto push_back(const T& value) noexcept -> void {
            reserve(_size + 1);
            _data[_size++] = value;
        }

        auto push_back(T&& value) noexcept -> void {
            reserve(_size + 1);
            _data[_size++] = kstd::forward<T>(value);
        }

        template<concepts::AssignableAs<T>... TArgs>
        constexpr auto push_back_all(TArgs&&... values) noexcept -> void {
            reserve(_size + sizeof...(TArgs));
            set_all_impl<TArgs...>(_size, kstd::forward<TArgs>(values)...);
        }

        auto pop_back() noexcept -> void {
            if(_size == 0) {
                panic("No elements in array");
            }
            _data[_size--].~T();
        }

        auto erase(iterator iter) noexcept -> void {
        }

        auto erase(const_iterator iter) const noexcept -> void {
        }

        [[nodiscard]] auto front() noexcept -> T& {
            if(_size == 0) {
                panic("No elements in array");
            }
            return _data[_size - 1];
        }

        [[nodiscard]] auto front() const noexcept -> const T& {
            if(_size == 0) {
                panic("No elements in array");
            }
            return _data[_size - 1];
        }

        [[nodiscard]] auto back() noexcept -> T& {
            if(_size == 0) {
                panic("No elements in array");
            }
            return _data[0];
        }

        [[nodiscard]] auto back() const noexcept -> const T& {
            if(_size == 0) {
                panic("No elements in array");
            }
            return _data[0];
        }

        template<typename... TArgs>
        auto emplace_back(TArgs&&... args) noexcept -> void {
            reserve(_size + 1);
            new(&_data[_size++]) T(kstd::forward<TArgs>(args)...);
        }

        [[nodiscard]] auto size() const noexcept -> usize {
            return _size;
        }

        [[nodiscard]] auto capacity() const noexcept -> usize {
            return _capacity;
        }

        [[nodiscard]] auto data() noexcept -> T* {
            return _data;
        }

        [[nodiscard]] auto data() const noexcept -> const T* {
            return _data;
        }

        [[nodiscard]] auto at(const usize index) noexcept -> T& {
            if(index >= _size) {
                panic("Array index out of bounds");
            }
            return _data[index];
        }

        [[nodiscard]] auto at(const usize index) const noexcept -> const T& {
            if(index >= _size) {
                panic("Array index out of bounds");
            }
            return _data[index];
        }

        [[nodiscard]] auto contains(const T& value) const noexcept -> bool {
            for(const auto& element : *this) {
                if(element != value) {
                    continue;
                }
                return true;
            }
            return false;
        }

        [[nodiscard]] auto operator[](const usize index) noexcept -> T& {
            return _data[index];
        }

        [[nodiscard]] auto operator[](const usize index) const noexcept -> const T& {
            return _data[index];
        }

        [[nodiscard]] auto operator==(const Array<T>& other) const noexcept -> bool {
            if(&other == this) {
                return true;
            }

            if(other._size != _size) {
                return false;
            }

            for(usize i = 0; i < _size; i++) {
                if(_data[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }
    };

    template<typename... TArgs, typename T = PackElement<0, TArgs...>>
    requires(are_assignable<T, TArgs...>)
    [[nodiscard]] constexpr auto array_of(TArgs&&... args) noexcept -> Array<T> {
        Array<T> array(sizeof...(TArgs));
        array.set_all(0, kstd::forward<TArgs>(args)...);
        return array;
    }
}// namespace kstd