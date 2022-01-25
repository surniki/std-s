
#include <stdio.h>
#include <string.h>
#include "test.h"

static int max(int n, int m)
{
	return (n > m) ? n : m;
}

static int is_test_entry_empty(const struct test_entry t)
{
	return t.test == NULL && t.name == NULL;
}

static void println_row(char c, int width)
{
	for (int i = 0; i < width; i++) {
		putc(c, stdout);
	}
	putc('\n', stdout);
}

int test_main(const struct test_entry *tests)
{
	int max_name_length = 0;
	int test_count = 0;
	int space_width = 8;
	const char *passed_msg = "PASSED";
	const char *failed_msg = "FAILED";	
	int max_status_len = max(strlen(passed_msg), strlen(failed_msg));

	for (const struct test_entry *t = tests; !is_test_entry_empty(*t); t++) {
		int current_name_length = strlen(t->name);
		if (max_name_length < current_name_length) {
			max_name_length = current_name_length;
		}
		++test_count;
	}

	int print_width = max_name_length + space_width + max_status_len + 1;

	int tests_passed = 0;
	if (test_count > 0) {
		printf("Running %d tests:\n", test_count);
		println_row('-', print_width);

		for (const struct test_entry *t = tests; !is_test_entry_empty(*t); t++) {
			int test_result = t->test();
			printf("%*s %*s\n",
			       max_name_length,
			       t->name,
			       space_width + max_status_len,
			       test_result ? passed_msg : failed_msg);
			if (test_result) {
				tests_passed++;
			}
		}

		println_row('-', print_width);
	}
	
	printf("(%d/%d) tests passed. %s\n",
	       tests_passed,
	       test_count,
	       tests_passed == test_count ? "All tests passed." : "Some tests failed.");
	
	return tests_passed == test_count ? 0 : 1;
}
