
#ifndef DYNARRAY_H
#define DYNARRAY_H

#ifndef DYNARRAY_STARTING_SIZE
#define DYNARRAY_STARTING_SIZE 8
#endif

#ifndef DYNARRAY_NEXT_SIZE_EXPR
#define DYNARRAY_NEXT_SIZE_EXPR(size) ((size) * 2)
#endif

#define DYNARRAY_DEFINE(type)										\
	struct dynarray_##type {									\
		unsigned length, size;									\
		type buffer[];										\
	};												\
	struct dynarray_##type *dynarray_##type##_create()						\
	{												\
		struct dynarray_##type *da =								\
			memory_calloc(1, (sizeof *da) + (sizeof (type)) * DYNARRAY_STARTING_SIZE);	\
		da->size = DYNARRAY_STARTING_SIZE;							\
		return da;										\
	}												\
	void dynarray_##type##_push(struct dynarray_##type **da, const type *value)			\
	{												\
		if ((*da)->length >= (*da)->size) {							\
			(*da)->size = DYNARRAY_NEXT_SIZE_EXPR((*da)->size);				\
			memory_resize(*da, (sizeof **da) + (*da)->size * (sizeof (type)));		\
		}											\
		(*da)->buffer[(*da)->length++] = *value;						\
	}												\
	type dynarray_##type##_pop(struct dynarray_##type *da)						\
	{												\
		assert(da->length > 0);									\
		da->length--;										\
		return da->buffer[da->length];								\
	}												\
	void dynarray_##type##_destroy(struct dynarray_##type **da)					\
	{												\
		memory_free(*da);									\
		*da = (void *)0;									\
	}

#endif
