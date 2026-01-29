#include "lp/lp_port.h"

LP_NORETURN void lp_port_panic(const char* msg) {
  (void)msg;
  // Optionally breakpoint:
  // __asm volatile("bkpt #0");
  while (1) { /* halt */ }
}

#if LP_CFG_ENABLE_LOG
void lp_port_log(lp_log_level_t lvl, const char* msg) {
  (void)lvl; (void)msg;
  // implement via UART/SWO/etc.
}
#endif

