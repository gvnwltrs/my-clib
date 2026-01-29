#pragma once
#include "lp_platform.h"
#include "lp_status.h"
#include "lp_assert.h"

typedef struct {
  uint8_t* buf;
  size_t   cap;   // bytes
  size_t   head;  // write
  size_t   tail;  // read
  bool     full;
} lp_ring;

static LP_INLINE void lp_ring_init(lp_ring* r, void* mem, size_t cap) {
  LP_ASSERT(r);
  r->buf  = (uint8_t*)mem;
  r->cap  = cap;
  r->head = 0;
  r->tail = 0;
  r->full = false;
}

static LP_INLINE size_t lp_ring_len(const lp_ring* r) {
  LP_ASSERT(r);
  if (r->full) return r->cap;
  if (r->head >= r->tail) return r->head - r->tail;
  return r->cap - (r->tail - r->head);
}

static LP_INLINE size_t lp_ring_free(const lp_ring* r) {
  return r->cap - lp_ring_len(r);
}

lp_status_t lp_ring_push(lp_ring* r, const void* data, size_t n);
lp_status_t lp_ring_pop (lp_ring* r, void* out, size_t n);

