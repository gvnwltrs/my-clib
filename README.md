# My C-Lib (Portable C Library)

If you want a *portable, general-purpose C library* that works for both embedded (bare metal/RTOS) and application-land (Linux/macOS/Windows), the winning move is to design it like a **tiny “C standard library supplement”** with a **hard separation between pure core code and platform glue**.

## Design rules (the ones that keep it portable)

1. **No OS assumptions in core**

   * Core must not call `malloc/free`, `printf`, file I/O, sockets, threads, time, etc.
2. **Everything that *touches the world* goes behind `port/`**

   * Time, logging, memory allocation (if you allow it), atomics, mutex, file, random, etc.
3. **No hidden global state**

   * Prefer “context objects” you pass around. Global state is fine only if it’s explicitly configured and documented.
4. **Deterministic by default**

   * Embedded consumers need predictable memory + timing. Make heap usage opt-in.
5. **Compile-time feature flags**

   * `#define LIBC_PORT_HAS_MALLOC 1`, `LIBC_CFG_ENABLE_LOG 0`, etc.

## Folder structure that scales

This is a proven layout for portability and testability:

```
libcportable/
  include/
    lp/                 # public headers
      lp.h              # umbrella header
      lp_config.h       # user-configurable knobs
      lp_port.h         # platform interface (implemented per target)
      bytes.h
      ring.h
      vec.h
      str.h
      fmt.h
      hash.h
      crc.h
      arena.h
      result.h
      assert.h
  src/
    core/               # pure C, no platform deps
      bytes.c
      ring.c
      vec.c
      str.c
      fmt.c
      hash.c
      crc.c
      arena.c
  port/
    posix/
      lp_port.c
    freertos/
      lp_port.c
    baremetal/
      lp_port.c
  tests/
    unit/
    fuzz/
  examples/
  cmake/
  CMakeLists.txt
```

**Rule:** `src/core` may include only:

* `<stdint.h> <stddef.h> <stdbool.h> <string.h>` (maybe)
* your own `lp/*.h`

## The “port layer” contract (the whole trick)

Define a small interface that the library expects the platform to provide:

* logging (optional)
* time (optional)
* allocation (optional)
* critical section / mutex (optional)
* assert/panic hook (useful)

Example concepts (not code):

* `lp_port_panic(const char* msg)`
* `lp_port_log(level, msg)`
* `lp_port_now_us()`
* `lp_port_malloc/free` (if enabled)

Then your embedded build can stub or implement minimal versions, while your app build uses POSIX/stdlib.

## What to include in a “general purpose” portable C library

Start with modules that are useful everywhere and don’t need an OS:

### Tier 1: always useful, no platform deps

* **Byte utilities:** endian, bit ops, hex encode/decode, base64 (optional)
* **Ring buffer** (SPSC-friendly design)
* **Small containers:** `vec` (dynamic but can be backed by arena), `deque` optional
* **String view + safe string utils** (length-aware, no reliance on `\0`)
* **Formatting** (tiny `printf`-like to buffer, no stdout)
* **Hashing:** FNV-1a / xxhash-like (choose one), plus hash map later
* **CRC**: CRC16/CRC32 (common embedded need)
* **Result / status codes**: consistent error handling

### Tier 2: memory + performance helpers

* **Arena allocator** (the workhorse for embedded + apps)
* **Slab/pool allocator** (optional)
* **Fixed-capacity containers** (compile-time sizes or user-provided backing buffers)

### Tier 3: “platformy” modules behind `port/`

* logging sinks
* time/monotonic
* random
* filesystem (only if you truly need it)
* threads/locks (only as wrappers)

## Memory strategy (this is where most libs fail)

Make 3 allocation modes:

1. **Fully static:** user provides backing memory buffers
2. **Arena-backed:** user provides a big region; you sub-allocate
3. **Heap-backed (optional):** only when enabled + port provides malloc/free

This lets you use the same APIs in embedded and apps, only changing config.

## Error handling strategy

Pick one and be consistent:

* return `lp_status_t` everywhere (0 = OK, negative = error)
* provide `lp_result(T)` style macros if you want nicer ergonomics
* never `assert` in production paths unless it’s a programmer error

## Testing strategy (so it doesn’t rot)

* Run **host-based unit tests** for core modules (fast, deterministic)
* Add **fuzz tests** for parsers/formatters/encoders
* Add 1–2 embedded “smoke examples” per MCU/RTOS target

## Suggested v1 “MVP” scope

If you want a strong base quickly:

1. `lp_config.h` + `lp_port.h` interface
2. `arena` + `ring` + `bytes` + `strview` + `fmt_to_buffer`
3. `crc32`
4. minimal test harness + CI on Linux/macOS (and maybe cross-compile sanity)

