#include <stdio.h>
#include "vector.h"
#include <assert.h>
vector *vector_alloc(vector_elem_cpy elem_copy_func,
                     vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
                     {
    if(elem_copy_func == NULL){
        return NULL;
    }
    if(elem_free_func == NULL){
        return NULL;
    }
    vector* new_vector = (vector*) malloc(sizeof(vector));
    if(new_vector == NULL)
    {
        return NULL;
    }
    new_vector->capacity = VECTOR_INITIAL_CAP;
    new_vector->size = 0;
    new_vector->data = (void**) malloc(VECTOR_INITIAL_CAP * sizeof(void*));
    if(new_vector->data == NULL)
    {
        free(new_vector);
        exit(EXIT_FAILURE);
    }
    new_vector->elem_copy_func = elem_copy_func;
    new_vector->elem_cmp_func = elem_cmp_func;
    new_vector->elem_free_func = elem_free_func;
    return new_vector;
                     }
                     void vector_free(vector **p_vector){
    if (p_vector != NULL && *p_vector != NULL){
        for(size_t i =0; i < (*p_vector)->size; i++){
            //dereference & since the function gets a pointer as a parameter
            (*p_vector)->elem_free_func(&(((*p_vector)->data)[i]));
        }
        free((*p_vector)->data);
        free(*p_vector);
        *p_vector = NULL;
        p_vector = NULL;
    }
}






void *vector_at(const vector *vector, size_t ind){
    if (vector == NULL || vector->size <= ind){
        return NULL;
    }
    if (vector->data == NULL || vector->data[ind] == NULL){
        return NULL;
    }
    return vector->data[ind];
}

int vector_find(const vector *vector, const void *value){
    if (vector == NULL || value == NULL){
        return -1;
    }
    for(size_t i = 0; i < vector->size; i++){
        if(vector->elem_cmp_func((vector->data)[i],value) == 1){
            return i;
        }
    }
    return -1;
}
int vector_push_back(vector *vector, const void *value){
    if(vector == NULL || value == NULL){
        return 0;
    }
    size_t a = vector->size + 1;
    size_t b = vector->capacity;
    if ((double)a/(double)b > VECTOR_MAX_LOAD_FACTOR){
        vector->capacity *= VECTOR_GROWTH_FACTOR;
        void **new_data = (void **) malloc(vector->capacity * sizeof (void *));
        if (new_data == NULL){
            exit(EXIT_FAILURE);
        }
        for(size_t i = 0; i < (vector->size); i++){
            new_data[i] = vector->elem_copy_func(vector->data[i]);
            vector->elem_free_func(&(vector->data[i]));
        }
        free(vector->data);
        vector->data = new_data;
    }
    void *new_value = vector->elem_copy_func(value);
    if (new_value == NULL){
        return 0;
    }
    vector->size = vector->size + 1;
    (vector->data)[vector->size - 1] = new_value;
    return 1;
}

double vector_get_load_factor(const vector *vector){
    if (vector == NULL){
        return -1;
    }
    if (vector->capacity<=0){return -1;}
    return (double) vector->size /(double) vector->capacity;

}

int vector_erase(vector *vector, size_t ind){
    if(vector == NULL || ind >= vector->size){
        return 0;
    }
    size_t a = vector->size - 1;
    size_t b = vector->capacity;
    //vector->capacity !=
    //    VECTOR_INITIAL_CAP &&
    if ((double)a/(double)b < VECTOR_MIN_LOAD_FACTOR){
        vector->capacity = vector->capacity / VECTOR_GROWTH_FACTOR;
        void **new_data = (void **) malloc(vector->capacity * sizeof (void *));
        if (new_data == NULL){
            exit(EXIT_FAILURE);
        }
        for(size_t i = 0; i < (vector->size-1); i++){
            if (i >= ind){
                new_data[i] = vector->elem_copy_func(vector->data[i + 1]);
                vector->elem_free_func(&(vector->data[i + 1]));
            }
            else{
                new_data[i] = vector->elem_copy_func(vector->data[i]);
                vector->elem_free_func(&(vector->data[i]));
            }
        }
        vector->elem_free_func(&(vector->data[ind]));
        free(vector->data);
        vector->data = new_data;
    }
    else{
        for(size_t i = ind; i < (vector->size-1); i++){
            vector->data[i] = vector->elem_copy_func(vector->data[i + 1]);
        }
    }
    vector->size = vector->size - 1;
    return 1;
}



void vector_clear(vector *vector)
{
    if(vector == NULL){}
    else{
        for(size_t i = vector->size -1; i > 0; i--){
            vector_erase(vector, i);
        }
        vector_erase(vector, 0);
    }
}


