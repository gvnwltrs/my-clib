#pragma once
#include "lp_types.h"
#include "lp_status.h"

static LP_INLINE lp_strview lp_sv(const char* s) {
  // safe for null? treat null as empty
  if (!s) return (lp_strview){ .ptr = "", .len = 0 };
  size_t n = 0;
  while (s[n] != '\0') n++;
  return (lp_strview){ .ptr = s, .len = n };
}

static LP_INLINE bool lp_sv_eq(lp_strview a, lp_strview b) {
  if (a.len != b.len) return false;
  for (size_t i = 0; i < a.len; i++) if (a.ptr[i] != b.ptr[i]) return false;
  return true;
}

