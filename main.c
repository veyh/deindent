#include "args.h"
#include "core/defs.h"
#include "sds/sds.h"
#include "deindent.h"
#include "util.h"
#include <errno.h>

static struct {
  struct args_s args;
  char line[DEINDENT_MAX_LINE_LENGTH];
  char line_cache[DEINDENT_MAX_LINE_LENGTH];
  sds content;
  ssize_t min_indent;
} self;

/*sublime-c-static-fn-hoist-start*/
static int deindent_file(FILE *file);
static void deindent_read(FILE *file);
static void handle_write(char const *data, size_t len);
/*sublime-c-static-fn-hoist-end*/

int main(int argc, char *argv[]) {
  self.args = args_parse(argc, argv);

  if (self.args.error != NULL) {
    fprintf(stderr, "%s\n", self.args.error);
    exit(1);
  }

  if (self.args.file_path == NULL) {
    return deindent_file(stdin);
  }

  FILE *file = fopen(self.args.file_path, "rb");

  if (file == NULL) {
    fprintf(
      stderr, "failed to open file: %s: %s\n",
      self.args.file_path, strerror(errno)
    );

    exit(1);
  }

  deindent_file(file);
  fclose(file);

  return 0;
}

static int deindent_file(FILE *file) {
  deindent_read(file);

  deindent_print((struct deindent_print_s) {
    .content = self.content,
    .content_length = sdslen(self.content),
    .indent = self.min_indent,
    .write = handle_write,
  });

  sdsfree(self.content);
  return 0;
}

static void deindent_read(FILE *file) {
  self.min_indent = DEINDENT_MAX_LINE_LENGTH;
  self.content = sdsempty();

  for (;;) {
    char *p = fgets(self.line, sizeof(self.line) - 1, file);

    if (p == NULL) {
      break;
    }

    if (self.args.convert_tabs_to_spaces) {
      util_convert_tabs((struct util_convert_tabs_s) {
        .spaces_per_tab = self.args.spaces_per_tab,
        .line_buffer = self.line,
        .line_buffer_capacity = sizeof(self.line),
        .line_buffer_used = strlen(self.line),
        .cache_buffer = self.line_cache,
        .cache_buffer_capacity = sizeof(self.line_cache),
      });
    }

    ssize_t indent = deindent_measure((struct deindent_measure_s) {
      .line = self.line,
      .min_indent = self.min_indent,
    });

    if (indent >= 0) {
      self.min_indent = w_min_2(self.min_indent, indent);
    }

    self.content = sdscat(self.content, self.line);
  }
}

static void handle_write(char const *data, size_t len) {
  fwrite(data, 1, len, stdout);
}
