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
#include <kstd/FixedArray.hpp>

TEST(kstd_FixedArray, fixed_array_of) {
    using namespace kstd;

    constexpr auto values = fixed_array_of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    static_assert(is_same<typename decltype(values)::element_type, int>);
    static_assert(values.get_size() == 10);

    for(usize i = 0; i < values.get_size(); i++) {
        ASSERT_EQ(values[i], i + 1);
    }
}

TEST(kstd_FixedArray, fixed_array_of_template) {
    using namespace kstd;

    constexpr auto values = fixed_array_of<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>();
    static_assert(is_same<typename decltype(values)::element_type, int>);
    static_assert(values.get_size() == 10);

    for(usize i = 0; i < values.get_size(); i++) {
        ASSERT_EQ(values[i], i + 1);
    }
}