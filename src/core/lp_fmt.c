#include "lp/lp_fmt.h"

static LP_INLINE void lp__nul_terminate(lp_fmtbuf* fb) {
  if (fb && fb->dst && fb->cap) {
    size_t i = (fb->len < fb->cap) ? fb->len : (fb->cap - 1);
    fb->dst[i] = '\0';
  }
}

lp_status_t lp_fmt_append_bytes(lp_fmtbuf* fb, const void* data, size_t n) {
  if (!fb || (!fb->dst && fb->cap != 0)) return LP_ERR_INVALID;
  if (!data && n != 0) return LP_ERR_INVALID;
  if (fb->cap == 0) { fb->truncated = (n != 0); return LP_OK; }

  const uint8_t* src = (const uint8_t*)data;
  size_t rem = lp_fmt_remaining(fb);

  size_t to_copy = (n <= rem) ? n : rem;
  for (size_t i = 0; i < to_copy; i++) {
    fb->dst[fb->len + i] = (char)src[i];
  }
  fb->len += to_copy;

  if (to_copy != n) fb->truncated = true;
  lp__nul_terminate(fb);
  return LP_OK;
}

lp_status_t lp_fmt_append_cstr(lp_fmtbuf* fb, const char* s) {
  if (!s) s = "(null)";
  // manual strlen (no libc requirement)
  size_t n = 0;
  while (s[n] != '\0') n++;
  return lp_fmt_append_bytes(fb, s, n);
}

lp_status_t lp_fmt_append_sv(lp_fmtbuf* fb, lp_strview sv) {
  if (!sv.ptr && sv.len != 0) return LP_ERR_INVALID;
  return lp_fmt_append_bytes(fb, sv.ptr, sv.len);
}

lp_status_t lp_fmt_append_char(lp_fmtbuf* fb, char c) {
  return lp_fmt_append_bytes(fb, &c, 1);
}

// -------------------------
// Decimal conversion helpers
// -------------------------

static lp_status_t lp__append_u64_dec(lp_fmtbuf* fb, uint64_t v) {
  // Max digits for u64 is 20
  char tmp[20];
  size_t n = 0;

  if (v == 0) {
    return lp_fmt_append_char(fb, '0');
  }

  while (v != 0) {
    uint64_t q = v / 10u;
    uint64_t r = v - (q * 10u);
    tmp[n++] = (char)('0' + (char)r);
    v = q;
  }

  // reverse
  for (size_t i = 0; i < n / 2; i++) {
    char t = tmp[i];
    tmp[i] = tmp[n - 1 - i];
    tmp[n - 1 - i] = t;
  }

  return lp_fmt_append_bytes(fb, tmp, n);
}

lp_status_t lp_fmt_append_u32(lp_fmtbuf* fb, uint32_t v) {
  return lp__append_u64_dec(fb, (uint64_t)v);
}

lp_status_t lp_fmt_append_u64(lp_fmtbuf* fb, uint64_t v) {
  return lp__append_u64_dec(fb, v);
}

lp_status_t lp_fmt_append_i32(lp_fmtbuf* fb, int32_t v) {
  int64_t vv = (int64_t)v;
  if (vv < 0) {
    lp_status_t st = lp_fmt_append_char(fb, '-');
    if (st != LP_OK) return st;
    // careful: abs(INT32_MIN) overflow avoided by widening
    uint64_t u = (uint64_t)(-vv);
    return lp__append_u64_dec(fb, u);
  }
  return lp__append_u64_dec(fb, (uint64_t)vv);
}

lp_status_t lp_fmt_append_i64(lp_fmtbuf* fb, int64_t v) {
  if (v < 0) {
    lp_status_t st = lp_fmt_append_char(fb, '-');
    if (st != LP_OK) return st;

    // Handle INT64_MIN safely: -(INT64_MIN) overflows signed, so do it via unsigned.
    uint64_t u = (uint64_t)(~(uint64_t)v) + 1u; // two's complement magnitude
    return lp__append_u64_dec(fb, u);
  }
  return lp__append_u64_dec(fb, (uint64_t)v);
}

// -------------------------
// Hex conversion
// -------------------------

static lp_status_t lp__append_u64_hex(lp_fmtbuf* fb, uint64_t v, bool uppercase, uint32_t nybble_count) {
  const char* digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  // nybble_count up to 16 for u64, 8 for u32
  char tmp[16];
  size_t n = 0;

  if (nybble_count == 0) return LP_ERR_INVALID;

  // fixed-width if nybble_count provided; otherwise we'd trim leading zeros.
  // Here we keep fixed width for deterministic logs (common embedded preference).
  for (uint32_t i = 0; i < nybble_count; i++) {
    uint32_t shift = (nybble_count - 1u - i) * 4u;
    uint8_t nyb = (uint8_t)((v >> shift) & 0xFu);
    tmp[n++] = digits[nyb];
  }
  return lp_fmt_append_bytes(fb, tmp, n);
}

lp_status_t lp_fmt_append_hex_u32(lp_fmtbuf* fb, uint32_t v, bool uppercase) {
  return lp__append_u64_hex(fb, (uint64_t)v, uppercase, 8u);
}

lp_status_t lp_fmt_append_hex_u64(lp_fmtbuf* fb, uint64_t v, bool uppercase) {
  return lp__append_u64_hex(fb, v, uppercase, 16u);
}

lp_status_t lp_fmt_append_ptr(lp_fmtbuf* fb, const void* p) {
  // Print pointers as 0x + fixed-width hex depending on pointer size.
  lp_status_t st = lp_fmt_append_cstr(fb, "0x");
  if (st != LP_OK) return st;

  uintptr_t v = (uintptr_t)p;
  uint32_t nyb = (uint32_t)(sizeof(uintptr_t) * 2u);

  // nyb <= 16 on 64-bit
  const char* digits = "0123456789abcdef";
  char tmp[16];
  size_t n = 0;
  for (uint32_t i = 0; i < nyb; i++) {
    uint32_t shift = (nyb - 1u - i) * 4u;
    uint8_t d = (uint8_t)((v >> shift) & 0xFu);
    tmp[n++] = digits[d];
  }
  return lp_fmt_append_bytes(fb, tmp, n);
}
