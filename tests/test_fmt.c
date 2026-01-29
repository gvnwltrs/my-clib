#include "lp/lp.h"

static int g_fail = 0;
#define T_ASSERT(expr) do { if (!(expr)) { g_fail++; } } while (0)

static void test_basic_append(void) {
  char buf[32];
  lp_fmtbuf fb = lp_fmtbuf_make(buf, sizeof(buf));

  T_ASSERT(lp_fmt_append_cstr(&fb, "abc") == LP_OK);
  T_ASSERT(lp_fmt_append_char(&fb, '-') == LP_OK);
  T_ASSERT(lp_fmt_append_sv(&fb, lp_sv("xyz")) == LP_OK);
  T_ASSERT(fb.truncated == false);
  T_ASSERT(fb.len == 7);
  T_ASSERT(buf[0] == 'a' && buf[6] == 'z');
}

static void test_truncation(void) {
  char buf[8];
  lp_fmtbuf fb = lp_fmtbuf_make(buf, sizeof(buf));

  T_ASSERT(lp_fmt_append_cstr(&fb, "1234567") == LP_OK); // fills usable 7 bytes
  T_ASSERT(fb.truncated == false);
  T_ASSERT(fb.len == 7);
  T_ASSERT(buf[7] == '\0');

  T_ASSERT(lp_fmt_append_char(&fb, 'X') == LP_OK); // no room
  T_ASSERT(fb.truncated == true);
  T_ASSERT(fb.len == 7);
  T_ASSERT(buf[7] == '\0');
}

static void test_decimal(void) {
  char buf[64];
  lp_fmtbuf fb = lp_fmtbuf_make(buf, sizeof(buf));

  T_ASSERT(lp_fmt_append_u32(&fb, 0) == LP_OK);
  T_ASSERT(lp_fmt_append_char(&fb, ' ') == LP_OK);
  T_ASSERT(lp_fmt_append_u32(&fb, 12345) == LP_OK);
  T_ASSERT(lp_fmt_append_char(&fb, ' ') == LP_OK);
  T_ASSERT(lp_fmt_append_i32(&fb, -42) == LP_OK);

  // Expect "0 12345 -42"
  T_ASSERT(fb.truncated == false);
  T_ASSERT(lp_sv_eq(lp_sv(buf), lp_sv("0 12345 -42")));
}

static void test_hex_fixed_width(void) {
  char buf[64];
  lp_fmtbuf fb = lp_fmtbuf_make(buf, sizeof(buf));

  T_ASSERT(lp_fmt_append_hex_u32(&fb, 0xA1B2C3D4u, false) == LP_OK);
  T_ASSERT(lp_fmt_append_char(&fb, ' ') == LP_OK);
  T_ASSERT(lp_fmt_append_hex_u64(&fb, 0x0102030405060708ull, false) == LP_OK);

  T_ASSERT(lp_sv_eq(lp_sv(buf),
    lp_sv("a1b2c3d4 0102030405060708")));
}

int main(void) {
  test_basic_append();
  test_truncation();
  test_decimal();
  test_hex_fixed_width();
  return g_fail ? 1 : 0;
}
