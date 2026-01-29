#pragma once
#include "lp_platform.h"

typedef struct { const uint8_t* ptr; size_t len; } lp_span_u8;
typedef struct { uint8_t*       ptr; size_t len; } lp_span_u8_mut;

typedef struct { const char* ptr; size_t len; } lp_strview;

// A growable buffer abstraction (implementation may be arena-backed or heap-backed)
typedef struct {
  uint8_t* data;
  size_t   len;
  size_t   cap;
} lp_buf;

