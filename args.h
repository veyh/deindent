#ifndef __args_h__
#define __args_h__

#include "core/defs.h"

struct args_s {
  char const *error;
  char const *file_path;

  bool convert_tabs_to_spaces;
  size_t spaces_per_tab;
};

extern struct args_s args_parse(int argc, char **argv);
extern void args_dump(struct args_s const *args);

#endif // __args_h__
