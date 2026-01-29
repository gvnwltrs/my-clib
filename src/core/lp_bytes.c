#include "lp/lp_bytes.h"

// -------------------------
// Checked arithmetic
// -------------------------

lp_status_t lp_checked_add_size(size_t a, size_t b, size_t* out) {
  if (!out) return LP_ERR_INVALID;
  if (a > (SIZE_MAX - b)) return LP_ERR_OVERFLOW;
  *out = a + b;
  return LP_OK;
}

lp_status_t lp_checked_mul_size(size_t a, size_t b, size_t* out) {
  if (!out) return LP_ERR_INVALID;
  if (a == 0 || b == 0) { *out = 0; return LP_OK; }
  if (a > (SIZE_MAX / b)) return LP_ERR_OVERFLOW;
  *out = a * b;
  return LP_OK;
}

// -------------------------
// Endian helpers
// -------------------------

static LP_INLINE uint16_t lp__bswap16(uint16_t x) {
  return (uint16_t)((x >> 8) | (x << 8));
}

static LP_INLINE uint32_t lp__bswap32(uint32_t x) {
  return ((x & 0x000000FFu) << 24) |
         ((x & 0x0000FF00u) <<  8) |
         ((x & 0x00FF0000u) >>  8) |
         ((x & 0xFF000000u) >> 24);
}

static LP_INLINE uint64_t lp__bswap64(uint64_t x) {
  return ((x & 0x00000000000000FFull) << 56) |
         ((x & 0x000000000000FF00ull) << 40) |
         ((x & 0x0000000000FF0000ull) << 24) |
         ((x & 0x00000000FF000000ull) <<  8) |
         ((x & 0x000000FF00000000ull) >>  8) |
         ((x & 0x0000FF0000000000ull) >> 24) |
         ((x & 0x00FF000000000000ull) >> 40) |
         ((x & 0xFF00000000000000ull) >> 56);
}

static LP_INLINE bool lp__is_little_endian(void) {
  const uint16_t x = 1;
  return *((const uint8_t*)&x) == 1;
}

// Unaligned-safe loads via byte assembly (no aliasing/align UB)
uint16_t lp_load_u16_le(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return (uint16_t)((uint16_t)b[0] | ((uint16_t)b[1] << 8));
}

uint16_t lp_load_u16_be(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return (uint16_t)(((uint16_t)b[0] << 8) | (uint16_t)b[1]);
}

uint32_t lp_load_u32_le(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return (uint32_t)b[0]
      | ((uint32_t)b[1] << 8)
      | ((uint32_t)b[2] << 16)
      | ((uint32_t)b[3] << 24);
}

uint32_t lp_load_u32_be(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return ((uint32_t)b[0] << 24)
      | ((uint32_t)b[1] << 16)
      | ((uint32_t)b[2] << 8)
      |  (uint32_t)b[3];
}

uint64_t lp_load_u64_le(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return (uint64_t)b[0]
      | ((uint64_t)b[1] << 8)
      | ((uint64_t)b[2] << 16)
      | ((uint64_t)b[3] << 24)
      | ((uint64_t)b[4] << 32)
      | ((uint64_t)b[5] << 40)
      | ((uint64_t)b[6] << 48)
      | ((uint64_t)b[7] << 56);
}

uint64_t lp_load_u64_be(const void* p) {
  const uint8_t* b = (const uint8_t*)p;
  return ((uint64_t)b[0] << 56)
      | ((uint64_t)b[1] << 48)
      | ((uint64_t)b[2] << 40)
      | ((uint64_t)b[3] << 32)
      | ((uint64_t)b[4] << 24)
      | ((uint64_t)b[5] << 16)
      | ((uint64_t)b[6] << 8)
      |  (uint64_t)b[7];
}

void lp_store_u16_le(void* p, uint16_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)(v & 0xFFu);
  b[1] = (uint8_t)((v >> 8) & 0xFFu);
}

void lp_store_u16_be(void* p, uint16_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)((v >> 8) & 0xFFu);
  b[1] = (uint8_t)(v & 0xFFu);
}

void lp_store_u32_le(void* p, uint32_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)(v & 0xFFu);
  b[1] = (uint8_t)((v >> 8) & 0xFFu);
  b[2] = (uint8_t)((v >> 16) & 0xFFu);
  b[3] = (uint8_t)((v >> 24) & 0xFFu);
}

void lp_store_u32_be(void* p, uint32_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)((v >> 24) & 0xFFu);
  b[1] = (uint8_t)((v >> 16) & 0xFFu);
  b[2] = (uint8_t)((v >> 8) & 0xFFu);
  b[3] = (uint8_t)(v & 0xFFu);
}

void lp_store_u64_le(void* p, uint64_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)(v & 0xFFull);
  b[1] = (uint8_t)((v >> 8) & 0xFFull);
  b[2] = (uint8_t)((v >> 16) & 0xFFull);
  b[3] = (uint8_t)((v >> 24) & 0xFFull);
  b[4] = (uint8_t)((v >> 32) & 0xFFull);
  b[5] = (uint8_t)((v >> 40) & 0xFFull);
  b[6] = (uint8_t)((v >> 48) & 0xFFull);
  b[7] = (uint8_t)((v >> 56) & 0xFFull);
}

void lp_store_u64_be(void* p, uint64_t v) {
  uint8_t* b = (uint8_t*)p;
  b[0] = (uint8_t)((v >> 56) & 0xFFull);
  b[1] = (uint8_t)((v >> 48) & 0xFFull);
  b[2] = (uint8_t)((v >> 40) & 0xFFull);
  b[3] = (uint8_t)((v >> 32) & 0xFFull);
  b[4] = (uint8_t)((v >> 24) & 0xFFull);
  b[5] = (uint8_t)((v >> 16) & 0xFFull);
  b[6] = (uint8_t)((v >> 8) & 0xFFull);
  b[7] = (uint8_t)(v & 0xFFull);
}

// Note: bswap helpers retained for future use (hashing etc.)
LP_UNUSED(lp__bswap16);
LP_UNUSED(lp__bswap32);
LP_UNUSED(lp__bswap64);
LP_UNUSED(lp__is_little_endian);

