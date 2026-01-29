#include "lp/lp.h"

static int g_fail = 0;

#define T_ASSERT(expr) do { if (!(expr)) { g_fail++; } } while (0)

static void test_checked_math(void) {
  size_t out = 0;

  T_ASSERT(lp_checked_add_size(1, 2, &out) == LP_OK);
  T_ASSERT(out == 3);

  T_ASSERT(lp_checked_mul_size(3, 4, &out) == LP_OK);
  T_ASSERT(out == 12);

  // overflow: SIZE_MAX + 1
  T_ASSERT(lp_checked_add_size(SIZE_MAX, 1, &out) == LP_ERR_OVERFLOW);

  // overflow: (SIZE_MAX/2+1)*2
  T_ASSERT(lp_checked_mul_size((SIZE_MAX / 2) + 1, 2, &out) == LP_ERR_OVERFLOW);
}

static void test_endian_load_store(void) {
  uint8_t b[8] = {0};

  lp_store_u16_le(b, 0x1234u);
  T_ASSERT(b[0] == 0x34u && b[1] == 0x12u);
  T_ASSERT(lp_load_u16_le(b) == 0x1234u);

  lp_store_u16_be(b, 0x1234u);
  T_ASSERT(b[0] == 0x12u && b[1] == 0x34u);
  T_ASSERT(lp_load_u16_be(b) == 0x1234u);

  lp_store_u32_le(b, 0xA1B2C3D4u);
  T_ASSERT(lp_load_u32_le(b) == 0xA1B2C3D4u);

  lp_store_u32_be(b, 0xA1B2C3D4u);
  T_ASSERT(lp_load_u32_be(b) == 0xA1B2C3D4u);

  lp_store_u64_le(b, 0x0102030405060708ull);
  T_ASSERT(lp_load_u64_le(b) == 0x0102030405060708ull);

  lp_store_u64_be(b, 0x0102030405060708ull);
  T_ASSERT(lp_load_u64_be(b) == 0x0102030405060708ull);
}

int main(void) {
  test_checked_math();
  test_endian_load_store();
  return g_fail ? 1 : 0;
}

