
#include "exception.c"
#include "assert.c"

#ifdef DEFINE_TESTS
#include "test.h"

TEST(catch_assert) {
	volatile int caught_the_assert = 0;

	TRY	
		assert(0);
	CATCH (assert_failure_exception)
		caught_the_assert = 1;
	END_TRY;

	ASSERT_EQUAL(caught_the_assert, 1);
	TEST_PASS;
}

TEST(finally) {
	volatile int caught_the_assert_first = 0;
	volatile int caught_the_assert_second = 0;
	volatile int in_finally_first = 0;
	volatile int in_finally_second = 0;

	TRY
		assert(1);
	CATCH (assert_failure_exception)
		caught_the_assert_first = 1;
	FINALLY
		in_finally_first = 1;
	END_TRY;

	TRY
		assert(0);
	CATCH (assert_failure_exception)
		caught_the_assert_second = 1;
	FINALLY
		in_finally_second = 1;
	END_TRY;

	ASSERT_EQUAL(caught_the_assert_first, 0);
	ASSERT_EQUAL(in_finally_first, 1);
	ASSERT_EQUAL(caught_the_assert_second, 1);
	ASSERT_EQUAL(in_finally_second, 1);
	TEST_PASS;
}

const struct exception never_used_exception = { "Never used" };

TEST(otherwise) {
	volatile int in_otherwise = 0;
	volatile int caught_the_assert = 0;
	
	TRY
		assert(0);
	CATCH (never_used_exception)
		caught_the_assert = 1;
	OTHERWISE
		in_otherwise = 1;
	END_TRY;

	ASSERT_EQUAL(caught_the_assert, 0);
	ASSERT_EQUAL(in_otherwise, 1);
	TEST_PASS;
}

const struct exception example_exception = { "Thrown intentionally" };

void a(void)
{
	THROW (example_exception);
}

void b(void)
{
	a();
}

void c(void)
{
	b();
}

TEST(nesting) {
	volatile int caught_the_example = 0;

	TRY
		c();
	CATCH (example_exception)
		caught_the_example = 1;
	END_TRY;

	ASSERT_EQUAL(caught_the_example, 1);
	TEST_PASS;		
}

void x(void)
{
	THROW (example_exception);
}

static volatile int raised_properly = 0;

void y()
{
	TRY
		x();
	CATCH (example_exception)
		raised_properly = 1;
		RAISE;
	END_TRY;
}

TEST(raise) {
	volatile int caught_the_example = 0;

	TRY
		y();
	CATCH (example_exception)
		caught_the_example = 1;
	END_TRY;

	ASSERT_EQUAL(raised_properly, 1);
	ASSERT_EQUAL(caught_the_example, 1);
	TEST_PASS;
}

TEST_INCLUDE(TEST_ENTRY(catch_assert),
	     TEST_ENTRY(finally),
	     TEST_ENTRY(otherwise),
	     TEST_ENTRY(nesting),
	     TEST_ENTRY(raise))

#endif
