#ifndef __w_log_h__
#define __w_log_h__

#include "defs.h"

#if defined(__arm__) && defined(__MODULE__)
  #define __FILENAME__ (__MODULE__)

#elif defined(__GNUC__) && defined(__FILE_NAME__)
  #define __FILENAME__ (__FILE_NAME__)

#elif defined(_MSC_VER) && defined(__FILE__)
  #define __FILENAME__ \
    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#elif defined(__FILE__)
  #define __FILENAME__ \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#else
  #error unable to generate __FILENAME__ definition
#endif

#ifndef W_LOG_NEWLINE_CHAR
  #define W_LOG_NEWLINE_CHAR "\n"
#endif

#ifndef w_log_prefix_time
  #define w_log_prefix_time()
#endif

#ifndef w_err_prefix_time
  #define w_err_prefix_time()
#endif

#ifndef w_log_str
  #define w_log_str(ptr) (ptr)
#endif

#ifndef w_log_without_prefix
  #define w_log_without_prefix(format, ...) \
    printf(format, ## __VA_ARGS__)
#endif

#ifndef w_err_without_prefix
  #define w_err_without_prefix(format, ...) \
    fprintf(stderr, format, ## __VA_ARGS__)
#endif

#define add_loc(format) \
  "[%s:%d] [%s] " format, \
  __FILENAME__, \
  __LINE__, \
  __FUNCTION__

#define w_log(format, ...) { \
  w_log_prefix_time(); \
  w_log_without_prefix(add_loc(format), ## __VA_ARGS__); \
}

#define w_log_line(format, ...) { \
  w_log_prefix_time(); \
  w_log_without_prefix(add_loc(format W_LOG_NEWLINE_CHAR), ## __VA_ARGS__); \
}

#define w_err_line(format, ...) { \
  w_err_prefix_time(); \
  w_err_without_prefix(add_loc(format W_LOG_NEWLINE_CHAR), ## __VA_ARGS__); \
}

#define w_log_line_without_prefix(format, ...) \
  w_log_without_prefix(format W_LOG_NEWLINE_CHAR, ## __VA_ARGS__)

#define w_log_check(condition, label, format, ...) { \
  if (!(condition)) {                                \
    w_log_line(format, ## __VA_ARGS__);              \
    goto label;                                      \
  }                                                  \
}

#define w_err_check(condition, label, format, ...) { \
  if (!(condition)) {                                \
    w_err_line(format, ## __VA_ARGS__);              \
    goto label;                                      \
  }                                                  \
}

#define w_log_check_warn(condition, format, ...) { \
  if (!(condition)) {                              \
    w_log_line("WARN: " format, ## __VA_ARGS__);   \
  }                                                \
}

#define w_log_check_without_prefix(condition, label, format, ...) { \
  if (!(condition)) {                                               \
    w_log_line_without_prefix(format, ## __VA_ARGS__);              \
    goto label;                                                     \
  }                                                                 \
}

w_unused static inline void w_log_hex(uint8_t const *data, uint32_t len) {
  for (uint32_t i = 0; i < len; ++i) {
    w_log_without_prefix("%02X", data[i]);
  }
}

#if defined(_MSC_VER)
  #define w_log_assert(condition, format, ...) {\
    if (!(condition)) {\
      char _msg[1024];\
      snprintf(_msg, 1024, add_loc(format), ## __VA_ARGS__);\
      MessageBoxA(NULL, _msg, "Error", 0);\
      exit(1);\
    }\
  }
#endif

#endif // __w_log_h__
