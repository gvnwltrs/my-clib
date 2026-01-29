#pragma once

typedef int32_t lp_status_t;

enum {
  LP_OK            = 0,
  LP_ERR_UNKNOWN   = -1,
  LP_ERR_INVALID   = -2,
  LP_ERR_RANGE     = -3,
  LP_ERR_NOMEM     = -4,
  LP_ERR_OVERFLOW  = -5,
  LP_ERR_EMPTY     = -6,
  LP_ERR_FULL      = -7,
  LP_ERR_IO        = -8,
  LP_ERR_UNSUP     = -9,
};

