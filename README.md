# Victor — A Vector Implementation in C++

An educational implementation of a resizable contiguous container inspired by `std::vector`. `Victor<T>` manages raw storage and object lifetimes explicitly, with a custom iterator type for traversal and random access.

The implementation is intentionally incomplete. It is a project for exploring container internals, not a drop-in replacement for the standard library.

## Implemented functionality

- Default, size, copy, and move construction.
- Copy and move assignment, plus explicit destruction of stored objects.
- `push_back`, `pop_back`, `assign`, index-based `insert` and `erase`.
- Capacity management with `reserve` and `shrink_to_fit`.
- `size`, `capacity`, `max_size`, and `empty`.
- Element access through `operator[]`, `at`, `front`, `back`, and `data`.
- A custom iterator with arithmetic, comparison, dereference, and increment/decrement operations.

Storage is obtained with `::operator new`, elements are created with placement `new`, and their destructors are called explicitly before storage is released.

## Quick example

Include [Test/Victor.h](Test/Victor.h); it includes the template definitions from `Victor.hpp`.

```cpp
#include <algorithm>
#include <iostream>
#include "Test/Victor.h"

int main() {
    Victor<int> values;
    values.push_back(30);
    values.push_back(10);
    values.push_back(20);

    std::sort(values.begin(), values.end());

    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n'; // 10 20 30
}
```

Save this example as `example.cpp` in the repository root.

### GCC or Clang

```sh
git clone https://github.com/Kxen0X/Incomplete-std-vector-implementation-.git
cd Incomplete-std-vector-implementation-
# Save the example above as example.cpp first.
g++ -std=c++17 -Wall -Wextra -I. example.cpp -o victor-example
./victor-example
```

The example uses a C++17 build configuration and needs no third-party libraries.

### Visual Studio

Open `Test.sln` in Visual Studio 2022 with the C++ desktop workload. The project uses the v143 toolset. [Test/main.cpp](Test/main.cpp) is currently an empty entry point; add your own usage example there and include `"Victor.h"` instead of `"Test/Victor.h"`.

## API differences and limitations

- `insert(pos, value)` and `erase(pos)` take numeric indices, unlike the iterator-based `std::vector` overloads.
- `resize()` is declared but has no definition yet.
- `const_iterator` currently aliases a const iterator object, rather than an iterator over const elements. Const access needs more work; the const `front()`/`back()` overloads also call a non-const `at()`.
- Bounds checks are provided by `at()`, but not consistently by mutation operations. Call `pop_back()` only on a non-empty container and use valid indices.
- Allocation and reallocation do not provide the standard container's exception-safety guarantees. Some paths copy elements, so move-only types are not fully supported.
- Allocator support, `emplace_back`, and a comprehensive automated test suite are not included.

The iterator exposes random-access operations, but full standard-library conformance has not been established.

## Repository layout

| File | Responsibility |
| --- | --- |
| [Test/Victor.h](Test/Victor.h) | Container and iterator declarations |
| [Test/Victor.hpp](Test/Victor.hpp) | Template implementations |
| [Test/main.cpp](Test/main.cpp) | Entry point for local experiments |
| [Test/Test.vcxproj](Test/Test.vcxproj) | Visual Studio project |

## Further work

Implement `resize`, improve const correctness and exception safety, make headers self-contained, and add tests for non-trivial element types and boundary conditions.
