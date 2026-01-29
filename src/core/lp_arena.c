#include "lp/lp_arena.h"

static bool lp__is_pow2(size_t x) { return x && ((x & (x - 1u)) == 0); }

lp_status_t lp_arena_alloc(lp_arena* a, size_t size, size_t align, void** out) {
  if (!a || !out || !a->mem) return LP_ERR_INVALID;
  if (size == 0) { *out = NULL; return LP_OK; }
  if (align == 0) align = 1;
  if (!lp__is_pow2(align)) return LP_ERR_INVALID;

  size_t off = a->off;
  size_t mask = align - 1u;
  size_t aligned = (off + mask) & ~mask;

  // overflow / bounds checks
  if (aligned < off) return LP_ERR_OVERFLOW;
  if (aligned > a->cap) return LP_ERR_NOMEM;
  if (size > (a->cap - aligned)) return LP_ERR_NOMEM;

  *out = a->mem + aligned;
  a->off = aligned + size;
  return LP_OK;
}

