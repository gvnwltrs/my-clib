#pragma once
#include "lp_platform.h"
#include "lp_config.h"

typedef enum {
  LP_LOG_DEBUG = 0,
  LP_LOG_INFO  = 1,
  LP_LOG_WARN  = 2,
  LP_LOG_ERROR = 3,
} lp_log_level_t;

// Must exist on all platforms
LP_NORETURN void lp_port_panic(const char* msg);

// Optional
#if LP_CFG_ENABLE_LOG
void lp_port_log(lp_log_level_t lvl, const char* msg);
#endif

#if LP_CFG_ENABLE_TIME
uint64_t lp_port_now_us(void);
#endif

#if LP_CFG_ENABLE_ALLOC
void* lp_port_malloc(size_t sz);
void  lp_port_free(void* p);
#endif

