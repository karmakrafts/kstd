# kstd

The Karma Krafts portable standard library for C++20 and up.

## Features
While mainly aiming to be an almost drop-in replacement for the regular C++20 standard library,  
kstd also offers some additional features which are focused on improving debuggability and portability.  
The library also provides various portable implementations of not-yet fully implemented C++ proposals.

- **[P0122R7](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0122r7.pdf)**/kstd::Slice - bounds-safe views for sequences of
  objects
- **[P1208R6](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1208r6.pdf)**/kstd::SourceLocation - Adopt source_location for C++20
- **[P0645R10](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0645r10.html)**/kstd::String::format - Text Formatting with std::format (libfmt)
- kstd::StackTrace - a portable interface for unwinding the stack and capturing a stack trace using debug symbols
- [mimalloc](https://github.com/microsoft/mimalloc) as the global default memory allocator

## Requirements

### Windows

No additional dependencies are required on Windows.

### Linux

On Linux, it is usually required to install the following libraries:

- libunwind

For Debian based systems which use the apt package manager, the following command may be used:

```shell
sudo apt install libunwind8 libunwind-dev
```

### macOS

On macOS, all required libraries are already shipped with the system.

## Standard Compliance

The following matrix illustrates which types are drop-in replacements for certain std:: types.

- ❌ Not supported and no support planned
- ❎ Not supported but support planned
- 🛠️ Work in progress
- ✔️ Supported and mostly compliant*
- ✅ Supported and compliant

&ast; This can mean that certain associated type definitions within the type are not present or that a single function
is missing or named differently.

| Standard Type         | Replacement Type      | Status | Notes |
|-----------------------|-----------------------|--------|-------|
| std::source_location  | kstd::SourceLocation  | ✅     |       |
| std::span             | kstd::Slice           | ✔️    | 
| std::iterator         | kstd::Iterator        | ✔️    |
| std::reverse_iterator | kstd::ReverseIterator | ✔️    |
| std::vector           | kstd::Array           | 🛠️    |
| std::array            | kstd::FixedArray      | 🛠️    |
| std::deque            | kstd::Deque           | 🛠️    |
| std::string           | kstd::String          | 🛠️    |
| std::wstring          | kstd::WString         | 🛠️    |
| std::string_view      | kstd::StringView      | 🛠️    |
| std::wstring_view     | kstd::WStringView     | 🛠️    |
| std::initializer_list | kstd::Slice           | ❌     |
| std::aligned_storage  | kstd::FixedArray      | ❌     |