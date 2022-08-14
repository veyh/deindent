#ifndef __util_h__
#define __util_h__

#include "core/defs.h"

struct util_parse_long_s {
  bool ok;
  long value;
};

extern struct util_parse_long_s util_parse_long(char const *src);

struct util_convert_tabs_s {
  size_t spaces_per_tab;

  char *line_buffer;
  size_t line_buffer_capacity;
  size_t line_buffer_used;

  char *cache_buffer;
  size_t cache_buffer_capacity;
};

extern void util_convert_tabs(
    struct util_convert_tabs_s args);

#endif // __util_h__
