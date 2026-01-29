#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(_MSC_VER)
  #define LP_INLINE __forceinline
  #define LP_NORETURN __declspec(noreturn)
#else
  #define LP_INLINE inline __attribute__((always_inline))
  #define LP_NORETURN __attribute__((noreturn))
#endif

#define LP_UNUSED(x) ((void)(x))

#if __STDC_VERSION__ >= 201112L
  #define LP_STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)
#else
  // best-effort fallback
  #define LP_STATIC_ASSERT(expr, msg) typedef char lp_static_assert_##msg[(expr)?1:-1]
#endif

