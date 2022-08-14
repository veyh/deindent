#include "util.h"
#include <errno.h>
#include <limits.h>

struct util_parse_long_s util_parse_long(char const *src) {
  struct util_parse_long_s result = { .ok = false };

  if (src == NULL) {
    goto ret;
  }

  char *endptr = NULL;
  result.value = strtol(src, &endptr, 10);

  if (errno != 0) {
    goto ret;
  }

  if (endptr == src) {
    goto ret;
  }

  if (result.value == LONG_MAX) {
    goto ret;
  }

  result.ok = true;

ret:
  return result;
}

void util_convert_tabs(
    struct util_convert_tabs_s args) {
  size_t tab_count = 0;

  for (size_t i = 0; i < args.line_buffer_used; i++) {
    if (args.line_buffer[i] == '\t') {
      tab_count += 1;
    }
  }

  if (tab_count == 0) {
    return;
  }

  size_t new_length =
    args.line_buffer_used + (tab_count * args.spaces_per_tab);

  if (new_length > args.line_buffer_capacity
  or  new_length > args.cache_buffer_capacity) {
    return;
  }

  size_t offset = 0;

  for (size_t i = 0; i < args.line_buffer_used; i++) {
    char value = args.line_buffer[i];

    if (value == '\t') {
      for (size_t j = 0; j < args.spaces_per_tab; j++) {
        args.cache_buffer[offset + i] = ' ';
        offset += 1;
      }

      offset -= 1; // for the tab itself
    }

    else {
      args.cache_buffer[offset + i] = value;
    }
  }

  memcpy(args.line_buffer, args.cache_buffer, new_length);
  args.line_buffer_used += new_length;
}
