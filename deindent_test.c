#include "deindent.h"
#include "stb/stb_ds.h"
#include "sds/sds.h"

/*sublime-c-static-fn-hoist-start*/
static int setup(void **ts);
static int teardown(void **ts);
static void handle_write(char const *data, size_t size);
static void test_measures_zero_indent(void **ts);
static void test_measures_empty_indent(void **ts);
static void test_measures_indent(void **ts);
static void test_writes_empty_input(void **ts);
static void test_writes_single_line_as_is(void **ts);
static void test_keeps_windows_line_endings(void **ts);
static void test_deindents_single_line(void **ts);
static void test_skips_empty_line_1(void **ts);
static void test_skips_empty_line_2(void **ts);
static void test_deindents_many_lines(void **ts);
/*sublime-c-static-fn-hoist-end*/

#define assert_output(expected) \
  assert_string_equal((expected), self.output)

static struct {
  sds output;
} self;

static int setup(void **ts) {
  self.output = sdsempty();
  return 0;
}

static int teardown(void **ts) {
  sdsfree(self.output);
  return 0;
}

static void handle_write(char const *data, size_t size) {
  self.output = sdscatlen(self.output, data, size);
}

static void test_measures_zero_indent(void **ts) {
  ssize_t count = deindent_measure((struct deindent_measure_s) {
    DEINDENT_MEASURE_defaults,
    .line = "zero",
  });

  assert_int_equal(0, count);
}

static void test_measures_empty_indent(void **ts) {
  ssize_t count = deindent_measure((struct deindent_measure_s) {
    DEINDENT_MEASURE_defaults,
    .line = "",
  });

  assert_int_equal(-1, count);

  count = deindent_measure((struct deindent_measure_s) {
    DEINDENT_MEASURE_defaults,
    .line = "\n",
  });

  assert_int_equal(-1, count);

  count = deindent_measure((struct deindent_measure_s) {
    DEINDENT_MEASURE_defaults,
    .line = "  ",
  });

  assert_int_equal(-1, count);
}

static void test_measures_indent(void **ts) {
  ssize_t count = deindent_measure((struct deindent_measure_s) {
    DEINDENT_MEASURE_defaults,
    .line = "  two",
  });

  assert_int_equal(2, count);
}

static void test_writes_empty_input(void **ts) {
  sds content = sdsnew("");

  deindent_print((struct deindent_print_s) {
    .indent = 0,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("");
}

static void test_writes_single_line_as_is(void **ts) {
  sds content = sdsnew("foo");

  deindent_print((struct deindent_print_s) {
    .indent = 0,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("foo");
}

static void test_keeps_windows_line_endings(void **ts) {
  sds content = sdsnew("  foo\r\n");

  deindent_print((struct deindent_print_s) {
    .indent = 2,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("foo\r\n");
}

static void test_deindents_single_line(void **ts) {
  sds content = sdsnew("  foo");

  deindent_print((struct deindent_print_s) {
    .indent = 2,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("foo");
}

static void test_skips_empty_line_1(void **ts) {
  sds content = sdsnew("  ");

  deindent_print((struct deindent_print_s) {
    .indent = 10,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("  ");
}

static void test_skips_empty_line_2(void **ts) {
  sds content = sdsnew("\n");

  deindent_print((struct deindent_print_s) {
    .indent = 10,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output("\n");
}

static void test_deindents_many_lines(void **ts) {
  sds content = sdsnew(
    "\n"
    " \n"
    "  \n"
    "   \n"
    "  2 to 0\n"
    "   3 to 1\n"
    "    4 to 2\n"
  );

  deindent_print((struct deindent_print_s) {
    .indent = 2,
    .content = content,
    .content_length = sdslen(content),
    .write = handle_write,
  });
  sdsfree(content);

  assert_output(
    "\n"
    " \n"
    "\n"
    " \n"
    "2 to 0\n"
    " 3 to 1\n"
    "  4 to 2\n"
  );
}

int main(int argc, char const *argv[]) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(test_measures_zero_indent, setup, teardown),
    cmocka_unit_test_setup_teardown(test_measures_empty_indent, setup, teardown),
    cmocka_unit_test_setup_teardown(test_measures_indent, setup, teardown),
    cmocka_unit_test_setup_teardown(test_writes_empty_input, setup, teardown),
    cmocka_unit_test_setup_teardown(test_writes_single_line_as_is, setup, teardown),
    cmocka_unit_test_setup_teardown(test_keeps_windows_line_endings, setup, teardown),
    cmocka_unit_test_setup_teardown(test_deindents_single_line, setup, teardown),
    cmocka_unit_test_setup_teardown(test_skips_empty_line_1, setup, teardown),
    cmocka_unit_test_setup_teardown(test_skips_empty_line_2, setup, teardown),
    cmocka_unit_test_setup_teardown(test_deindents_many_lines, setup, teardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
