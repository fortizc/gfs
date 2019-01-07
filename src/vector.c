#include <utils/vector.h>

#include <stdlib.h>
#include <stdio.h>


struct _vector
{
    size_t size;
    size_t capacity;
    void **data;
};


void vector_resize(vector_t *v)
{
    if (v->size < v->capacity)
        return;

    v->capacity *= 2;
    v->data = (void **) realloc(v->data, sizeof(void *) * v->capacity);
}


vector_t *vector_new(size_t size)
{
    vector_t *v = (vector_t *) malloc(sizeof(vector_t));

    if (!v)
        return NULL;

    v->data = (void **) malloc(sizeof(void *) * size);

    if (!v->data)
    {
        free(v);
        return NULL;
    }

    v->capacity = size;
    v->size = 0;

    return v;
}


void vector_free(vector_t *v)
{
    free(v->data);
    free(v);
}


void vector_append(vector_t *v, void *data)
{
    vector_resize(v);
    v->data[v->size++] = data;
}


void *vector_get(vector_t *v, size_t index)
{
    if (index >= v->size)
    {
        fprintf(stderr, "Index out of bounds");
        exit(1);
    }

    return v->data[index];
}


void vector_set(vector_t *v, size_t index, void *data)
{
    if (index >= v->size)
    {
        fprintf(stderr, "Index out of bounds");
        exit(1);
    }

    v->data[index] = data;
}


size_t vector_get_size(vector_t *v)
{
    return v->size;
}

void vector_shrink_to_fit(vector_t *v)
{
    for (size_t i = v->size; i < v->capacity; ++i)
        free(v->data[i]);
}
