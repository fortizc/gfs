#ifndef GFS_VECTOR_H
#define GFS_VECTOR_H

#include <stddef.h>

struct _vector;
typedef struct _vector vector_t;

vector_t *vector_new(size_t size);
void vector_free(vector_t *v);
void vector_append(vector_t *v, void *data);
void *vector_get(vector_t *v, size_t index);
void vector_set(vector_t *v, size_t index, void *data);
size_t vector_get_size(vector_t *v);
void vector_shrink_to_fit(vector_t *v);

#endif
