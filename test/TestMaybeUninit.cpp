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
#include <kstd/MaybeUninit.hpp>

TEST(kstd_MaybeUninit, create) {
    using namespace kstd;
    MaybeUninit<Array<usize>> values {};
}

TEST(kstd_MaybeUninit, set) {
    using namespace kstd;
    MaybeUninit<Array<usize>> values {};

    values = array_of<usize>(0, 1, 2, 3, 4);

    const auto& array = values.assume_init();
    ASSERT_EQ(array.size(), 5);
    for(usize i = 0; i < array.size(); ++i) {
        ASSERT_EQ(array[i], i);
    }
}