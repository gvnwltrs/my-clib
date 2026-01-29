#include "lp/lp_port.h"
#include <stdio.h>
#include <stdlib.h>

LP_NORETURN void lp_port_panic(const char* msg) {
  fprintf(stderr, "PANIC: %s\n", msg ? msg : "(null)");
  abort();
}

#if LP_CFG_ENABLE_LOG
void lp_port_log(lp_log_level_t lvl, const char* msg) {
  const char* tag = (lvl==LP_LOG_ERROR)?"ERR":(lvl==LP_LOG_WARN)?"WRN":(lvl==LP_LOG_INFO)?"INF":"DBG";
  fprintf(stderr, "[%s] %s\n", tag, msg ? msg : "(null)");
}
#endif

#if LP_CFG_ENABLE_ALLOC
void* lp_port_malloc(size_t sz) { return malloc(sz); }
void  lp_port_free(void* p) { free(p); }
#endif

