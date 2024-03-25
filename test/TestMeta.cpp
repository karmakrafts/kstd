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

#include <kstd/Meta.hpp>
#include <kstd/String.hpp>

using namespace kstd;

static_assert(is_same<void, void>);
static_assert(is_same<bool, bool>);
static_assert(is_same<const char*, const char*>);
static_assert(!is_same<const char*, char*>);

static_assert(is_assignable<u64, u32>);
static_assert(is_assignable<u32, u64>);
static_assert(!is_assignable<void*, bool>);
static_assert(is_assignable<void*, bool*>);
static_assert(!is_assignable<bool*, void*>);

static_assert(is_convertible<void*, int*>);
static_assert(is_convertible<String, const char*>);
static_assert(!is_convertible<const char*, String>);

static_assert(is_same<remove_const<const int>, int>);
static_assert(is_same<remove_const<const int*>, int*>);
static_assert(is_same<remove_const<const int&>, int&>);

static_assert(is_same<remove_ptr<const int*>, const int>);
static_assert(is_same<remove_ptr<int*>, int>);
static_assert(is_same<remove_ptr<void*>, void>);

static_assert(is_same<remove_ref<const int&>, const int>);
static_assert(is_same<remove_ref<int&>, int>);
static_assert(is_same<remove_ref<int*&>, int*>);