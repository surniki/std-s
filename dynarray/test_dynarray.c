
#include "assert.h"
#include "memory.h"

#define DYNARRAY_STARTING_SIZE 2
#define DYNARRAY_NEXT_SIZE_EXPR(size) ((size) + DYNARRAY_STARTING_SIZE)
#include "dynarray.h"

#ifdef DEFINE_TESTS
#include "test.h"

DYNARRAY_DEFINE(int);

typedef struct {
	float x, y, z;
} point3;

DYNARRAY_DEFINE(point3);

TEST(dynarray_int_create_destroy) {	
	struct dynarray_int *da = dynarray_int_create();
	
	ASSERT_EQUAL(da->size, 2);
	ASSERT_EQUAL(da->length, 0);
	ASSERT_FALSE(da->buffer == (void *)0);

	dynarray_int_destroy(&da);

	ASSERT_EQUAL(da, (void *)0);
	
	TEST_PASS;
}

TEST(dynarray_int_push) {
	int x = 3;
	int y = 4;
	int z = 5;	
	struct dynarray_int *da = dynarray_int_create();

	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);
	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);
	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);

	ASSERT_EQUAL(da->size, 10);
	ASSERT_EQUAL(da->length, 9);

	ASSERT_EQUAL(da->buffer[0], 3);
	ASSERT_EQUAL(da->buffer[1], 4);
	ASSERT_EQUAL(da->buffer[2], 5);
	ASSERT_EQUAL(da->buffer[3], 3);
	ASSERT_EQUAL(da->buffer[4], 4);
	ASSERT_EQUAL(da->buffer[5], 5);
	ASSERT_EQUAL(da->buffer[6], 3);
	ASSERT_EQUAL(da->buffer[7], 4);
	ASSERT_EQUAL(da->buffer[8], 5);

	dynarray_int_destroy(&da);
	
	TEST_PASS;
}

TEST(dynarray_int_pop) {
	int x = 3;
	int y = 4;
	int z = 5;	
	struct dynarray_int *da = dynarray_int_create();

	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);
	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);
	dynarray_int_push(&da, &x);
	dynarray_int_push(&da, &y);
	dynarray_int_push(&da, &z);

	int r1 = dynarray_int_pop(da);
	int r2 = dynarray_int_pop(da);
	int r3 = dynarray_int_pop(da);

	ASSERT_EQUAL(r1, 5);
	ASSERT_EQUAL(r2, 4);
	ASSERT_EQUAL(r3, 3);
	ASSERT_EQUAL(da->length, 6);
	ASSERT_EQUAL(da->size, 10);
}

TEST(dynarray_point3_create_destroy) {
	struct dynarray_point3 *da = dynarray_point3_create();
	
	ASSERT_EQUAL(da->size, 2);
	ASSERT_EQUAL(da->length, 0);
	ASSERT_FALSE(da->buffer == (void *)0);

	dynarray_point3_destroy(&da);

	ASSERT_EQUAL(da, (void *)0);
	
	TEST_PASS;
}

TEST(dynarray_point3_push) {
	point3 x = { .x = 1.0f, .y = 2.0f, .z = 3.0f };
	point3 y = { .x = 4.0f, .y = 5.0f, .z = 6.0f };
	point3 z = { .x = 7.0f, .y = 8.0f, .z = 9.0f };
	struct dynarray_point3 *da = dynarray_point3_create();

	dynarray_point3_push(&da, &x);
	dynarray_point3_push(&da, &y);
	dynarray_point3_push(&da, &z);

	ASSERT_EQUAL(da->size, 4);
	ASSERT_EQUAL(da->length, 3);

	ASSERT_EQUAL(da->buffer[0].x, 1.0f);
	ASSERT_EQUAL(da->buffer[0].y, 2.0f);
	ASSERT_EQUAL(da->buffer[0].z, 3.0f);
	ASSERT_EQUAL(da->buffer[1].x, 4.0f);
	ASSERT_EQUAL(da->buffer[1].y, 5.0f);
	ASSERT_EQUAL(da->buffer[1].z, 6.0f);
	ASSERT_EQUAL(da->buffer[2].x, 7.0f);
	ASSERT_EQUAL(da->buffer[2].y, 8.0f);
	ASSERT_EQUAL(da->buffer[2].z, 9.0f);

	dynarray_point3_destroy(&da);
	
	TEST_PASS;
}

TEST(dynarray_point3_pop) {
	point3 x = { .x = 1.0f, .y = 2.0f, .z = 3.0f };
	point3 y = { .x = 4.0f, .y = 5.0f, .z = 6.0f };
	point3 z = { .x = 7.0f, .y = 8.0f, .z = 9.0f };
	struct dynarray_point3 *da = dynarray_point3_create();

	dynarray_point3_push(&da, &x);
	dynarray_point3_push(&da, &y);
	dynarray_point3_push(&da, &z);

	point3 r = dynarray_point3_pop(da);

	ASSERT_EQUAL(r.x, 7.0f);
	ASSERT_EQUAL(r.y, 8.0f);
	ASSERT_EQUAL(r.z, 9.0f);
	ASSERT_EQUAL(da->length, 2);
	ASSERT_EQUAL(da->size, 4);

	dynarray_point3_destroy(&da);
	
	TEST_PASS;
}

TEST_INCLUDE(TEST_ENTRY(dynarray_int_create_destroy),
	     TEST_ENTRY(dynarray_int_push),
	     TEST_ENTRY(dynarray_int_pop),
	     TEST_ENTRY(dynarray_point3_create_destroy),
	     TEST_ENTRY(dynarray_point3_push),
	     TEST_ENTRY(dynarray_point3_pop));
	     
#endif
