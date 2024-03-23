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
#include <kstd/StackTrace.hpp>
#include <istream>

TEST(kstd_StackTrace, get_current) {
    using namespace kstd;

    const auto stack_trace = StackTrace::get_current();
    for (usize i = 0; i < stack_trace.get_depth(); i++) {
        const auto& element = stack_trace[i];

        std::cout << std::hex << "Address: " << element.get_address() << '\n';
        printf("Binary: %s\n", element.get_binary().c_str());
        printf("File Name: %s\n", element.get_file_name().c_str());
        printf("Function Name: %s\n", element.get_function_name().c_str());
        printf("Line: %d\n", static_cast<int>(element.get_line()));
        printf("Column: %d\n", static_cast<int>(element.get_column()));
        printf("\n");
    }
}