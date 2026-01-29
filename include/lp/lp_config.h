#pragma once

// Build mode integration
#ifndef LP_CFG_DEBUG
  #if !defined(NDEBUG)
    #define LP_CFG_DEBUG 1
  #else
    #define LP_CFG_DEBUG 0
  #endif
#endif

// Enable/disable optional subsystems
#ifndef LP_CFG_ENABLE_LOG
  #define LP_CFG_ENABLE_LOG 1
#endif

#ifndef LP_CFG_ENABLE_TIME
  #define LP_CFG_ENABLE_TIME 0
#endif

#ifndef LP_CFG_ENABLE_ALLOC
  // default off: embedded-friendly
  #define LP_CFG_ENABLE_ALLOC 0
#endif

// Hard limits / defaults
#ifndef LP_CFG_FMT_TMP_SIZE
  #define LP_CFG_FMT_TMP_SIZE 128u
#endif

