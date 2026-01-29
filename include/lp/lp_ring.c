#include "lp/lp_ring.h"
#include <string.h>

static void lp__ring_advance(lp_ring* r, size_t* idx, size_t n) {
  *idx = (*idx + n) % r->cap;
}

lp_status_t lp_ring_push(lp_ring* r, const void* data, size_t n) {
  if (!r || !data) return LP_ERR_INVALID;
  if (n == 0) return LP_OK;
  if (r->cap == 0) return LP_ERR_RANGE;
  if (n > lp_ring_free(r)) return LP_ERR_FULL;

  const uint8_t* src = (const uint8_t*)data;
  size_t first = r->cap - r->head;
  if (first > n) first = n;

  memcpy(&r->buf[r->head], src, first);
  memcpy(&r->buf[0], src + first, n - first);

  lp__ring_advance(r, &r->head, n);
  r->full = (r->head == r->tail);
  return LP_OK;
}

lp_status_t lp_ring_pop(lp_ring* r, void* out, size_t n) {
  if (!r || !out) return LP_ERR_INVALID;
  if (n == 0) return LP_OK;
  if (n > lp_ring_len(r)) return LP_ERR_EMPTY;

  uint8_t* dst = (uint8_t*)out;
  size_t first = r->cap - r->tail;
  if (first > n) first = n;

  memcpy(dst, &r->buf[r->tail], first);
  memcpy(dst + first, &r->buf[0], n - first);

  lp__ring_advance(r, &r->tail, n);
  r->full = false;
  return LP_OK;
}

