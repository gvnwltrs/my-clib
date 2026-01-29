#pragma once
#include "lp_platform.h"
#include "lp_status.h"

typedef struct {
  char*  dst;
  size_t cap;
  size_t len; // written (excluding null)
  bool   truncated;
} lp_fmtbuf;

static LP_INLINE lp_fmtbuf lp_fmtbuf_make(char* dst, size_t cap) {
  lp_fmtbuf fb = { .dst = dst, .cap = cap, .len = 0, .truncated = false };
  if (dst && cap) dst[0] = '\0';
  return fb;
}

lp_status_t lp_fmt_append_str(lp_fmtbuf* fb, const char* s);
lp_status_t lp_fmt_append_u32(lp_fmtbuf* fb, uint32_t v);

