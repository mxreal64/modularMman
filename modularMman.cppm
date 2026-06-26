module;

// Ensure GNU extensions are active for mremap and advanced advice flags
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/mman.h>
#include <unistd.h>

export module modularMman;

import std;

export namespace Sys::mman {

    using Address = void;

    // Protections
    enum class MemoryProt : int {
        None  = PROT_NONE,
        Read  = PROT_READ,
        Write = PROT_WRITE,
        Exec  = PROT_EXEC
    };

    // Mapping flags
    enum class MapFlags : int {
        Shared    = MAP_SHARED,
        Private   = MAP_PRIVATE,
        Anonymous = MAP_ANONYMOUS,
        Fixed     = MAP_FIXED,
        Populate  = MAP_POPULATE,
        Locked    = MAP_LOCKED
    };

    // 1. New: msync flags for explicit cache flushing control
    enum class SyncFlags : int {
        Async      = MS_ASYNC,      // Schedule background disk write
        Sync       = MS_SYNC,       // Block thread execution until flush completes
        Invalidate = MS_INVALIDATE  // Drop other file mappings to update cache
    };

    // 2. New: madvise flags for explicit page-fault strategy choices
    enum class MemoryAdvice : int {
        Normal     = MADV_NORMAL,     // Default kernel heuristic tracking
        Random     = MADV_RANDOM,     // Suppress background read-ahead passes
        Sequential = MADV_SEQUENTIAL, // Aggressively pre-fetch downstream pages
        WillNeed   = MADV_WILLNEED,   // Force swap-in page table instantiation
        DontNeed   = MADV_DONTNEED,   // Instantly clear physical tracking allocations
        HugePage   = MADV_HUGEPAGE,   // Optimize memory backing using Transparent Huge Pages
        NoHugePage = MADV_NOHUGEPAGE  // Enforce rigid standard 4KB target boundary maps
    };

    // 3. New: mremap configuration flags
    enum class RemapFlags : int {
        None      = 0,
        MayMove   = MREMAP_MAYMOVE, // Allow kernel to shift location if expansion space is blocked
        Fixed     = MREMAP_FIXED    // Enforce target layout to fixed destination boundaries
    };

    // Operators
    constexpr MemoryProt operator|(MemoryProt lhs, MemoryProt rhs) noexcept { return static_cast<MemoryProt>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
    constexpr MapFlags operator|(MapFlags lhs, MapFlags rhs) noexcept { return static_cast<MapFlags>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
    constexpr SyncFlags operator|(SyncFlags lhs, SyncFlags rhs) noexcept { return static_cast<SyncFlags>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
    constexpr RemapFlags operator|(RemapFlags lhs, RemapFlags rhs) noexcept { return static_cast<RemapFlags>(static_cast<int>(lhs) | static_cast<int>(rhs)); }

    // --- High-Performance Inline API ---
    
    [[nodiscard]] inline Address* map(Address* hint_addr, std::size_t length, MemoryProt prot, MapFlags flags, int fd, off_t offset) noexcept {
        return ::mmap(hint_addr, length, static_cast<int>(prot), static_cast<int>(flags), fd, offset);
    }

    inline int unmap(Address* addr, std::size_t length) noexcept {
        return ::munmap(addr, length);
    }

    [[nodiscard]] inline int protect(Address* addr, std::size_t length, MemoryProt prot) noexcept {
        return ::mprotect(addr, length, static_cast<int>(prot));
    }

    [[nodiscard]] inline int lock(const Address* addr, std::size_t length) noexcept {
        return ::mlock(addr, length);
    }

    inline int unlock(const Address* addr, std::size_t length) noexcept {
        return ::munlock(addr, length);
    }

    // New API Additions for Core Completeness
    [[nodiscard]] inline int sync(Address* addr, std::size_t length, SyncFlags flags) noexcept {
        return ::msync(addr, length, static_cast<int>(flags));
    }

    [[nodiscard]] inline int advise(Address* addr, std::size_t length, MemoryAdvice advice) noexcept {
        return ::madvise(addr, length, static_cast<int>(advice));
    }

    [[nodiscard]] inline Address* remap(Address* old_addr, std::size_t old_length, std::size_t new_length, RemapFlags flags, Address* new_addr = nullptr) noexcept {
        return ::mremap(old_addr, old_length, new_length, static_cast<int>(flags), new_addr);
    }

    template<typename T>
	[[nodiscard]] inline bool is_failed(T* addr) noexcept {
    	return reinterpret_cast<void*>(addr) == MAP_FAILED;
	}
}
