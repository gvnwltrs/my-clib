#pragma once
#include "lp_platform.h"
#include "lp_status.h"

/*
  lp_bytes: portable byte/endian utilities + checked arithmetic helpers.
  - Core-only: no OS, no malloc, no stdio.
  - No UB on overflow: checked ops return LP_ERR_OVERFLOW.
*/

// -------------------------
// Checked arithmetic
// -------------------------

lp_status_t lp_checked_add_size(size_t a, size_t b, size_t* out);
lp_status_t lp_checked_mul_size(size_t a, size_t b, size_t* out);

// -------------------------
// Endian load/store (unaligned-safe)
// -------------------------

uint16_t lp_load_u16_le(const void* p);
uint16_t lp_load_u16_be(const void* p);
uint32_t lp_load_u32_le(const void* p);
uint32_t lp_load_u32_be(const void* p);
uint64_t lp_load_u64_le(const void* p);
uint64_t lp_load_u64_be(const void* p);

void lp_store_u16_le(void* p, uint16_t v);
void lp_store_u16_be(void* p, uint16_t v);
void lp_store_u32_le(void* p, uint32_t v);
void lp_store_u32_be(void* p, uint32_t v);
void lp_store_u64_le(void* p, uint64_t v);
void lp_store_u64_be(void* p, uint64_t v);

// -------------------------
// Bit utilities
// -------------------------

static LP_INLINE uint32_t lp_rotl32(uint32_t x, uint32_t r) {
  r &= 31u;
  return (x << r) | (x >> ((32u - r) & 31u));
}

static LP_INLINE uint32_t lp_rotr32(uint32_t x, uint32_t r) {
  r &= 31u;
  return (x >> r) | (x << ((32u - r) & 31u));
}

static LP_INLINE uint64_t lp_rotl64(uint64_t x, uint32_t r) {
  r &= 63u;
  return (x << r) | (x >> ((64u - r) & 63u));
}

static LP_INLINE uint64_t lp_rotr64(uint64_t x, uint32_t r) {
  r &= 63u;
  return (x >> r) | (x << ((64u - r) & 63u));
}

static LP_INLINE bool lp_is_pow2_size(size_t x) {
  return x && ((x & (x - 1u)) == 0);
}

static LP_INLINE size_t lp_align_up_size(size_t x, size_t align) {
  // Precondition: align is power of two (use lp_is_pow2_size to verify)
  return (x + (align - 1u)) & ~(align - 1u);
}

