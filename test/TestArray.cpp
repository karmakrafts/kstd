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
#include <kstd/Array.hpp>
#include <kstd/StringView.hpp>

TEST(kstd_Array, array_of) {
    using namespace kstd;
    auto values = array_of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    static_assert(is_same<typename decltype(values)::element_type, i32>);
    ASSERT_EQ(values.size(), 10);
    for(usize i = 0; i < values.size(); ++i) {
        ASSERT_EQ(values[i], i);
    }
}

TEST(kstd_Array, push_back) {
    using namespace kstd;
    Array<usize> values {};
    for(usize i = 0; i < 10; ++i) {
        values.push_back(i);
        ASSERT_EQ(values.size(), i + 1);
        ASSERT_EQ(values[i], i);
    }
}

TEST(kstd_Array, pop_back) {
    using namespace kstd;
    auto values = array_of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    for(usize i = 0; i < 5; ++i) {
        ASSERT_EQ(values.size(), 10 - i);
        values.pop_back();
    }
    ASSERT_EQ(values.size(), 5);
    for(usize i = 0; i < 5; ++i) {
        ASSERT_EQ(values[i], i);
    }
}

TEST(kstd_Array, emplace_back) {
    using namespace kstd;
    Array<usize> values {};
}

TEST(kstd_Array, back) {
    using namespace kstd;
    Array<usize> values {};
}

TEST(kstd_Array, front) {
    using namespace kstd;
    Array<usize> values {};
}

TEST(kstd_Array, insert) {
    using namespace kstd;

    auto values = array_of(0, 1, 2, 3, 4, 6, 7, 8, 9);
    values.insert(5, 5);

    ASSERT_EQ(values, array_of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST(kstd_Array, insert_empty) {
    using namespace kstd;

    Array<int> values;
    values.insert(0, 1);

    ASSERT_EQ(values, array_of(1));
}

TEST(kstd_Array, insert_end) {
    using namespace kstd;

    auto values = array_of(0, 1, 2, 3, 4, 5, 6);
    values.insert(7, 7);

    ASSERT_EQ(values, array_of(0, 1, 2, 3, 4, 5, 6, 7));
}

TEST(kstd_Array, insert_all) {
    using namespace kstd;

    auto values = array_of(0, 1, 2, 6, 7, 8, 9);
    values.insert_all(3, 3, 4, 5);

    ASSERT_EQ(values, array_of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST(kstd_Array, insert_all_empty) {
    using namespace kstd;

    Array<int> values;
    values.insert_all(0, 1, 2, 3, 4, 5);

    ASSERT_EQ(values, array_of(1, 2, 3, 4, 5));
}

TEST(kstd_Array, insert_all_end) {
    using namespace kstd;

    auto values = array_of(0, 1, 2, 3, 4, 5, 6);
    values.insert_all(7, 7, 8, 9);

    ASSERT_EQ(values, array_of(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}