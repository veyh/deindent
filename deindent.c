#include "deindent.h"

struct printer_s {
  struct deindent_print_s args;
  char const *content_end;

  char const *line_start;
  size_t line_length;

  char const *next_line_start;
};

/*sublime-c-static-fn-hoist-start*/
static struct printer_s printer_create(struct deindent_print_s args);
static void printer_print(struct printer_s *self);
static void printer_find_next_line(struct printer_s *self);
static void printer_deindent_line(struct printer_s *self);
static void printer_write_line(struct printer_s *self);
static void printer_advance(struct printer_s *self);
/*sublime-c-static-fn-hoist-end*/

ssize_t deindent_measure(struct deindent_measure_s args) {
  ssize_t indent = 0;
  ssize_t len = strnlen(args.line, DEINDENT_MAX_LINE_LENGTH);

  for (
    ssize_t i = 0;
    i < len and (args.min_indent < 0 or i < args.min_indent);
    i++
  ) {
    if (args.line[i] == '\n') {
      return -1;
    }

    else if (args.line[i] == '\r'
    and (i + 1) < len
    and args.line[i + 1] == '\n') {
      return -1;
    }

    else if (args.line[i] == ' ') {
      indent += 1;
    }

    else {
      // A character that isn't a space, \t, \r or \n was found, so this is not
      // just an empty line.
      return indent;
    }
  }

  // If we get here, it was an empty line without line endings.
  return -1;
}

void deindent_print(struct deindent_print_s args) {
  struct printer_s self = printer_create(args);
  printer_print(&self);
}

static struct printer_s printer_create(struct deindent_print_s args) {
  struct printer_s self = {
    .args = args,
    .content_end = args.content + args.content_length,
    .line_start = args.content,
  };

  return self;
}

static void printer_print(struct printer_s *self) {
  while (self->line_start < self->content_end) {
    printer_find_next_line(self);
    printer_deindent_line(self);
    printer_write_line(self);
    printer_advance(self);
  }
}

static void printer_find_next_line(struct printer_s *self) {
  self->next_line_start = strchr(self->line_start, '\n');

  if (self->next_line_start == NULL) {
    self->next_line_start = self->content_end;
  }

  else {
    self->next_line_start += 1; // for the \n
  }
}

static void printer_deindent_line(struct printer_s *self) {
  self->line_length = self->next_line_start - self->line_start;

  if (self->line_length > self->args.indent) {
    self->line_start += self->args.indent;
    self->line_length -= self->args.indent;
  }
}

static void printer_write_line(struct printer_s *self) {
  self->args.write(self->line_start, self->line_length);
}

static void printer_advance(struct printer_s *self) {
  self->line_start = self->next_line_start;
}
