#ifndef __deindent_h__
#define __deindent_h__

#include "core/defs.h"

// Surely nobody has lines longer than this... right?
#define DEINDENT_MAX_LINE_LENGTH 10240

#define DEINDENT_MEASURE_defaults \
  .min_indent = -1

struct deindent_measure_s {
  char const *line;
  ssize_t min_indent;
};

extern ssize_t deindent_measure(struct deindent_measure_s args);

struct deindent_print_s {
  size_t indent;
  char const *content;
  size_t content_length;
  void (*write) (char const *data, size_t size);
};

extern void deindent_print(struct deindent_print_s args);

#endif // __deindent_h__
