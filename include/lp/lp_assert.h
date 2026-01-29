#pragma once
#include "lp_config.h"
#include "lp_platform.h"
#include "lp_port.h"

#if LP_CFG_DEBUG
  #define LP_ASSERT(expr) do { if (!(expr)) lp_port_panic("LP_ASSERT: " #expr); } while (0)
#else
  #define LP_ASSERT(expr) do { (void)sizeof(expr); } while (0)
#endif

