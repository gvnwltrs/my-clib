#pragma once
#include "lp_platform.h"
#include "lp_status.h"
#include "lp_strview.h"

/*
  lp_fmt: deterministic formatting into a preallocated buffer.

  Guarantees:
  - Never writes past cap.
  - Always keeps dst NUL-terminated if cap > 0.
  - Tracks truncation.
  - No heap, no stdio, core-only.
*/

typedef struct {
  char*  dst;
  size_t cap;
  size_t len; // written (excluding null)
  bool   truncated; // set if append couldn't fit everything
} lp_fmtbuf;

static LP_INLINE lp_fmtbuf lp_fmtbuf_make(char* dst, size_t cap) {
  lp_fmtbuf fb = { .dst = dst, .cap = cap, .len = 0, .truncated = false };
  if (dst && cap) dst[0] = '\0';
  return fb;
}

static LP_INLINE size_t lp_fmt_remaining(const lp_fmtbuf* fb) {
  if (!fb || fb->cap == 0) return 0;
  // reserve 1 for NUL
  if (fb->len >= fb->cap) return 0;
  size_t usable = fb->cap - 1;
  return (fb->len >= usable) ? 0 : (usable - fb->len);
}

// Low-level append primitives
lp_status_t lp_fmt_append_bytes(lp_fmtbuf* fb, const void* data, size_t n);
lp_status_t lp_fmt_append_cstr(lp_fmtbuf* fb, const char* s);
lp_status_t lp_fmt_append_sv(lp_fmtbuf* fb, lp_strview sv);
lp_status_t lp_fmt_append_char(lp_fmtbuf* fb, char c);

// Numeric (decimal)
lp_status_t lp_fmt_append_u32(lp_fmtbuf* fb, uint32_t v);
lp_status_t lp_fmt_append_u64(lp_fmtbuf* fb, uint64_t v);
lp_status_t lp_fmt_append_i32(lp_fmtbuf* fb, int32_t v);
lp_status_t lp_fmt_append_i64(lp_fmtbuf* fb, int64_t v);

// Hex (no prefix)
lp_status_t lp_fmt_append_hex_u32(lp_fmtbuf* fb, uint32_t v, bool uppercase);
lp_status_t lp_fmt_append_hex_u64(lp_fmtbuf* fb, uint64_t v, bool uppercase);

// Convenience: hex with 0x prefix
lp_status_t lp_fmt_append_ptr(lp_fmtbuf* fb, const void* p);