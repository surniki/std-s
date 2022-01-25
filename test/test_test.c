
#include "test.c"

#ifdef DEFINE_TESTS

TEST(max) {
	ASSERT_EQUAL(max(-4, -10), -4);
	ASSERT_EQUAL(max(4, 10), 10);
	ASSERT_EQUAL(max(-4, 10), 10);
	ASSERT_EQUAL(max(4, -10), 4);
	ASSERT_EQUAL(max(0, 0), 0);
	ASSERT_EQUAL(max(4, 10), max(10, 4));

	TEST_PASS;
}

TEST(example) {
	TEST_PASS;
}

TEST(is_test_entry_empty) {
	struct test_entry set_entry = TEST_ENTRY(example);
	struct test_entry blank_entry = TEST_NULL_ENTRY;

	ASSERT_FALSE(is_test_entry_empty(set_entry));
	ASSERT_TRUE(is_test_entry_empty(blank_entry));

	TEST_PASS;
}

TEST_INCLUDE(TEST_ENTRY(max), TEST_ENTRY(is_test_entry_empty))

#endif
