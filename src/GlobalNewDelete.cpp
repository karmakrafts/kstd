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

#ifdef KSTD_GLOBAL_MIMALLOC

#include <mimalloc.h>
#include <new>

// NOLINTBEGIN
void* operator new(const size_t size) {
    return mi_malloc(size);
}

void* operator new[](const size_t size) {
    return mi_malloc(size);
}

void* operator new(const size_t size, std::align_val_t alignment) {
    return mi_malloc_aligned(size, static_cast<size_t>(alignment));
}

void* operator new[](const size_t size, std::align_val_t alignment) {
    return mi_malloc_aligned(size, static_cast<size_t>(alignment));
}

void operator delete(void* memory) {
    mi_free(memory);
}

void operator delete[](void* memory) {
    mi_free(memory);
}
// NOLINTEND

#endif//KSTD_GLOBAL_MIMALLOC