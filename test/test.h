
#ifndef TEST_H
#define TEST_H

struct test_entry {
	int (*test)(void);
	const char *name;	
};

int test_main(const struct test_entry *tests);

#define TEST_TO_STR(n) "test_"#n
#define TEST_TO_FP(n) &test_##n
#define TEST_ENTRY(n) (struct test_entry){ .test = TEST_TO_FP(n), .name = TEST_TO_STR(n) }
#define TEST_NULL_ENTRY  (const struct test_entry){ .test = (void *)0, .name = (void *)0 }

#define TEST_PASS return 1
#define TEST_FAIL return 0
#define TEST(name) int test_##name(void)

#define ASSERT_TRUE(expr)      do { if (!(expr)) TEST_FAIL; } while (0)
#define ASSERT_FALSE(expr)     do { if ((expr)) TEST_FAIL; } while (0)
#define ASSERT_EQUAL(x, y)     do { if ((x) != (y)) TEST_FAIL; } while (0)
#define ASSERT_NOT_EQUAL(x, y) do { if ((x) == (y)) TEST_FAIL; } while (0)

#define TEST_INCLUDE(...) int main(void) { const struct test_entry internalname__entries[] = { __VA_ARGS__, TEST_NULL_ENTRY }; return test_main(internalname__entries); }

#endif
