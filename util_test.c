#include "util.h"

/*sublime-c-static-fn-hoist-start*/
static void test_parses_long(void **ts);
/*sublime-c-static-fn-hoist-end*/

#define assert_parse_long_fail(input) \
  assert_false(util_parse_long(input).ok);

#define assert_parse_long_ok(input, expected) \
  assert_true(util_parse_long(input).ok); \
  assert_int_equal(expected, util_parse_long(input).value);

static void test_parses_long(void **ts) {
  assert_parse_long_fail(NULL);
  assert_parse_long_fail("");
  assert_parse_long_ok("-1", -1);
  assert_parse_long_ok("-2", -2);
  assert_parse_long_ok("0", 0);
  assert_parse_long_ok("1", 1);
  assert_parse_long_ok("2", 2);
  assert_parse_long_ok("10", 10);
}

static void test_replaces_tabs_with_space(void **ts) {
  char buffer[200];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, "\tfoo");

  char cache_buffer[200];

  util_convert_tabs((struct util_convert_tabs_s) {
    .spaces_per_tab = 16,
    .line_buffer = buffer,
    .line_buffer_capacity = sizeof(buffer),
    .line_buffer_used = strlen(buffer),
    .cache_buffer = cache_buffer,
    .cache_buffer_capacity = sizeof(cache_buffer),
  });

  assert_string_equal("                foo", buffer);
}

static void test_does_not_replace_tabs_if_replacement_doesnt_fit_in_line_buffer(void **ts) {
  char buffer[10];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, "\tfoo");

  char cache_buffer[100];

  util_convert_tabs((struct util_convert_tabs_s) {
    .spaces_per_tab = 16,
    .line_buffer = buffer,
    .line_buffer_capacity = sizeof(buffer),
    .line_buffer_used = strlen(buffer),
    .cache_buffer = cache_buffer,
    .cache_buffer_capacity = sizeof(cache_buffer),
  });

  assert_string_equal("\tfoo", buffer);
}

static void test_does_not_replace_tabs_if_replacement_doesnt_fit_in_cache_buffer(void **ts) {
  char buffer[100];
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, "\tfoo");

  char cache_buffer[10];

  util_convert_tabs((struct util_convert_tabs_s) {
    .spaces_per_tab = 16,
    .line_buffer = buffer,
    .line_buffer_capacity = sizeof(buffer),
    .line_buffer_used = strlen(buffer),
    .cache_buffer = cache_buffer,
    .cache_buffer_capacity = sizeof(cache_buffer),
  });

  assert_string_equal("\tfoo", buffer);
}

int main(int argc, char const *argv[]) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_parses_long),
    cmocka_unit_test(test_replaces_tabs_with_space),
    cmocka_unit_test(test_does_not_replace_tabs_if_replacement_doesnt_fit_in_line_buffer),
    cmocka_unit_test(test_does_not_replace_tabs_if_replacement_doesnt_fit_in_cache_buffer),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
