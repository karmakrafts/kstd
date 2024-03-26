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
#include "Panic.hpp"
#include "Types.hpp"

namespace kstd {
    template<typename T>
    struct DequeNode final {
        T data;
        DequeNode* last;
        DequeNode* next;
    };

    template<typename T, concepts::Allocator<DequeNode<T>> TAllocator = Allocator<DequeNode<T>>>
    struct Deque final {
        using value_type = T;
        using allocator_type = TAllocator;
        using node_type = DequeNode<T>;

    private:
        class Iterator final {
            node_type* _current_node;

        public:
            explicit Iterator(node_type* current_node) noexcept
                : _current_node(current_node) {
            }

            [[nodiscard]] auto operator==(const Iterator& other) const noexcept -> bool {
                return _current_node == other._current_node;
            }

            auto operator++() noexcept -> Iterator& {
                _current_node = _current_node->next;
                return *this;
            }

            auto operator*() const noexcept -> const T& {
                return _current_node->data;
            }

            auto operator*() noexcept -> T& {
                return _current_node->data;
            }

            auto operator->() noexcept -> T* {
                return _current_node->data;
            }
        };

    public:
        using iterator_type = Iterator;
        using const_iterator_type = const Iterator;

    private:
        TAllocator _allocator;
        node_type* _first_node;
        node_type* _last_node;
        usize _size;

    public:
        Deque(usize count, T value) noexcept
            : _allocator()
            , _first_node(nullptr)
            , _last_node(nullptr)
            , _size(0) {
            for(usize i = 0; i < count; i++) {
                push_back(value);
            }
        }

        Deque() noexcept
            : _allocator()
            , _first_node(nullptr)
            , _last_node(nullptr)
            , _size(0) {
        }

        auto insert(const usize index, T value) noexcept -> void {
            if (index > _size) {
                panic("Unable to insert element because out of bounds");
            }

            // Walk node tree to node at index
            node_type* node = _first_node;
            for (usize i = 0; i < index - 1; i++) {
                node = node->next;
            }

            // Create new node
            node_type* new_node = _allocator.allocate(1);
            new_node->last = node;
            new_node->next = node->next;
            new_node->data = std::move(value);

            // Insert node
            if (node->next != nullptr) {
                node->next->last = new_node;
            } else {
                _last_node = new_node;
            }
            node->next = new_node;
            _size++;
        }

        auto push_front(const T value) noexcept -> void {
            node_type* node = _allocator.allocate(1);
            new(node) node_type();
            node->data = std::move(value);
            if(_first_node == nullptr) {
                _first_node = _last_node = node;
            }
            else {
                node->next = _first_node;
                _first_node->last = node;
                _first_node = node;
            }
            _size++;
        }

        auto pop_front() noexcept -> T {
            if(_first_node == nullptr) {
                panic("Unable to pop front because no elements are inserted");
            }

            auto value = _first_node->data;
            _first_node = _first_node->next;
            return std::move(value);
        }

        auto peek_front() const noexcept -> const T& {
            if(_first_node == nullptr) {
                panic("Unable to pop front because no elements are inserted");
            }
            return _first_node->data;
        }

        auto peek_front() noexcept -> T& {
            if(_first_node == nullptr) {
                panic("Unable to pop front because no elements are inserted");
            }
            return _first_node->data;
        }

        auto push_back(const T value) noexcept -> void {
            node_type* node = _allocator.allocate(1);
            new(node) node_type();
            node->data = std::move(value);
            if(_last_node == nullptr) {
                _first_node = _last_node = node;
            }
            else {
                node->last = _last_node;
                _last_node->next = node;
                _last_node = node;
            }
            _size++;
        }

        auto pop_back() noexcept -> T {
            if(_last_node == nullptr) {
                panic("Unable to pop back because no elements are inserted");
            }

            auto value = std::move(_last_node->data);
            _last_node = _last_node->last;
            return std::move(value);
        }

        auto peek_back() const noexcept -> const T& {
            if(_first_node == nullptr) {
                panic("Unable to pop back because no elements are inserted");
            }
            return _first_node->data;
        }

        auto peek_back() noexcept -> T& {
            if(_first_node == nullptr) {
                panic("Unable to pop back because no elements are inserted");
            }
            return _first_node->data;
        }

        [[nodiscard]] auto operator[](const usize index) const noexcept -> const T& {
            auto node = _first_node;
            for(usize i = 0; i < index; i++) {
                node = node->next;
            }
            return node->data;
        }

        [[nodiscard]] auto operator[](const usize index) noexcept -> T& {
            auto node = _first_node;
            for(usize i = 0; i < index; i++) {
                node = node->next;
            }
            return node->data;
        }

        [[nodiscard]] inline auto cbegin() const noexcept -> Iterator {
            return Iterator(_first_node);
        }

        [[nodiscard]] inline auto cend() const noexcept -> Iterator {
            return Iterator(_last_node->next);
        }

        [[nodiscard]] inline auto begin() noexcept -> Iterator {
            return Iterator(_first_node);
        }

        [[nodiscard]] inline auto end() noexcept -> Iterator {
            return Iterator(_last_node->next);
        }
    };
}// namespace kstd