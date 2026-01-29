#pragma once
#include "lp_platform.h"
#include "lp_status.h"
#include "lp_assert.h"

typedef struct {
  uint8_t* mem;
  size_t   cap;
  size_t   off;
} lp_arena;

static LP_INLINE void lp_arena_init(lp_arena* a, void* mem, size_t cap) {
  LP_ASSERT(a);
  a->mem = (uint8_t*)mem;
  a->cap = cap;
  a->off = 0;
}

static LP_INLINE void lp_arena_reset(lp_arena* a) {
  LP_ASSERT(a);
  a->off = 0;
}

lp_status_t lp_arena_alloc(lp_arena* a, size_t size, size_t align, void** out);

