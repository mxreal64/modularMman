# modularMman

A blazing-fast, zero-overhead C++26 named module gateway for raw Linux virtual memory management. It performs a brutal macro-exorcism to purge toxic, legacy POSIX preprocessor pollution from your global namespace while delivering direct, type-safe kernel performance.

## Why It Matters

Traditional `<sys/mman.h>` usage floods your codebase with unsafe global macros (`MAP_PRIVATE`, `PROT_READ`, etc.) and forces error-prone integer flag masking. 

`modularMman` firewalls the legacy C headers inside the global module fragment and exposes strongly typed, modern C++ structures that compile down to the exact same assembly as raw system calls.

## Features

* **Zero Memory Overhead:** Pure `inline` API with zero heap allocation or runtime abstraction penalties.
* **Strong Type Safety:** Replaces loose integer flags with strict, bitmask-supported `enum class` types for protections, flags, and memory advice.
* **Advanced Cache Control:** Direct support for Transparent Huge Pages (`MemoryAdvice::HugePage`) and precise kernel prefetching heuristics.
* **Compile-Time Isolation:** Fully isolates POSIX macros so they can never bleed into your downstream user code.

## Quick Example

```cpp
import modularMman;
import std;

int main() {
    using namespace Sys::mman;

    size_t page_size = 4096;
    
    // Allocate a raw anonymous page pool
    void* buffer = map(
        nullptr, 
        page_size, 
        MemoryProt::Read | MemoryProt::Write, 
        MapFlags::Private | MapFlags::Anonymous, 
        -1, 
        0
    );

    if (is_failed(buffer)) {
        std::println(stderr, "Mapping failed!");
        return 1;
    }

    // Advise the kernel to aggressively prefetch or optimize page tables
    advise(buffer, page_size, MemoryAdvice::HugePage);

    // Clean up
    unmap(buffer, page_size);
    return 0;
}
```

## License

copyright mxreal64, 2026

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
