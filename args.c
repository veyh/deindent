#include "args.h"
#include "core/w_log.h"
#include "core/xmalloc.h"
#include "commander/src/commander.h"
#include "util.h"

#ifdef LINTING
  #define DEINDENT_VERSION "dev"
#else
  #include "deindent_version.h"
#endif

/*sublime-c-static-fn-hoist-start*/
static void cmd_tabs(command_t *self);
/*sublime-c-static-fn-hoist-end*/

struct args_s args_parse(int argc, char **argv) {
  struct args_s res = {
    .file_path = NULL,
  };

  command_t cmd;
  command_init(&cmd, argv[0], DEINDENT_VERSION);

  cmd.data = &res;
  cmd.usage = "[options] [file]\n"
              "\n  "
              "If file is omitted, reads from stdin.";

  command_option(
    &cmd, "-t", "--tabs <arg>", "convert tabs to spaces", cmd_tabs
  );
  command_parse(&cmd, argc, argv);

  for (int i = 0; i < cmd.argc; ++i) {
    res.file_path = xstrdup(cmd.argv[i]);
    break;
  }

  command_free(&cmd);
  return res;
}

void args_dump(struct args_s const *args) {
  w_log_line("error: %s", args->error);
  w_log_line("file_path: %s", args->file_path);
  w_log_line("spaces_per_tab: %zu", args->spaces_per_tab);
}

static void cmd_tabs(command_t *cmd) {
  struct util_parse_long_s result = util_parse_long(cmd->arg);

  // Let's be reasonable :D
  if (result.ok and 0 < result.value and result.value < 100) {
    struct args_s *args = cmd->data;
    args->convert_tabs_to_spaces = true;
    args->spaces_per_tab = result.value;
  }
}
