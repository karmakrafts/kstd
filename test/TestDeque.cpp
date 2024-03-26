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

#include <gtest/gtest.h>
#include "kstd/Deque.hpp"

TEST(kstd_Deque, push_back) {
    auto deque = kstd::Deque<uint32_t>();
    deque.push_back(1337);
    deque.push_back(1338);
    ASSERT_EQ(deque.pop_back(), 1338);
    ASSERT_EQ(deque.pop_back(), 1337);
}

TEST(kstd_Deque, push_front) {
    auto deque = kstd::Deque<uint32_t>();
    deque.push_front(1338);
    deque.push_front(1337);
    ASSERT_EQ(deque.pop_back(), 1338);
    ASSERT_EQ(deque.pop_back(), 1337);
}

TEST(kstd_Deque, index) {
    auto deque = kstd::Deque<uint32_t>();
    deque.push_front(1338);
    ASSERT_EQ(deque[0], 1338);
}

TEST(kstd_Deque, insert_as_last) {
    auto deque = kstd::Deque<uint32_t>();
    deque.push_back(1337);
    deque.insert(1, 1338);
    deque.push_back(1339);
    ASSERT_EQ(deque[0], 1337);
    ASSERT_EQ(deque[1], 1338);
    ASSERT_EQ(deque[2], 1339);
}

TEST(kstd_Deque, insert) {
    auto deque = kstd::Deque<uint32_t>();
    deque.push_back(1337);
    deque.push_back(1339);
    deque.insert(1, 1338);
    ASSERT_EQ(deque[0], 1337);
    ASSERT_EQ(deque[1], 1338);
    ASSERT_EQ(deque[2], 1339);
}
