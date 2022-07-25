#include <stdlib.h>
#include "hashmap.h"
#include <assert.h>
#include <stdio.h>


hashmap *hashmap_alloc (hash_func func){
    if(func == NULL)
    {
        return NULL;
    }
    hashmap* hash_map = (hashmap*) malloc(sizeof(hashmap));
    if(hash_map == NULL)
    {
        exit(EXIT_FAILURE);
    }
    //buckets
    hash_map->buckets = (vector**) malloc(HASH_MAP_INITIAL_CAP
            * sizeof(vector*));
    if(hash_map->buckets == NULL)
    {
        free(hash_map);
        exit(EXIT_FAILURE);
    }
    //size
    hash_map->size = 0;
    //capacity
    hash_map->capacity = HASH_MAP_INITIAL_CAP;
    //hash_func
    hash_map->hash_func = func;
    for(size_t i = 0; i < HASH_MAP_INITIAL_CAP; i++){
        hash_map->buckets[i] = NULL;
    }
    return hash_map;
}


void hashmap_free(hashmap **p_hash_map){
    if (p_hash_map == NULL || *p_hash_map == NULL){}
    else{
        for (size_t i = 0; i < (*p_hash_map)->capacity;i++){
            vector_free(&(*p_hash_map)->buckets[i]);
        }
        free((*p_hash_map)->buckets);
        free(*p_hash_map);
        *p_hash_map = NULL;
        p_hash_map = NULL;
    }
}


int new_hash_map(hashmap *hash_map, size_t old_capacity, size_t new_capacity){
    vector** new_array_vec = (vector**) malloc(new_capacity
            * sizeof(vector*));
    if(new_array_vec == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for(size_t k = 0; k < new_capacity; k++){
        new_array_vec[k] = NULL;
    }
    for(size_t i = 0; i < old_capacity; i++)
    {
        //we search the elements at the hash table
        if(hash_map->buckets[i] != NULL)
        {
            for(size_t j = 0; j < hash_map->buckets[i]->size; j++)
            {
                size_t index = hash_map->hash_func(((pair*)
                        (hash_map->buckets[i]->data[j]))->key) & (new_capacity - 1);
                if(new_array_vec[index] == NULL)
                {
                    //checks if the function vector_alloc fails
                    new_array_vec[index] = vector_alloc(pair_copy,
                                                        pair_cmp, pair_free);
                    if(new_array_vec[index] == NULL){
                        for(size_t t = 0; t < new_capacity; t++)
                        {
                            //frees all the vectors one by one
                            //vector_free parameter is a pointer to pointer
                            // that's why we do & to the pointer
                            vector_free(&(new_array_vec[t]));
                        }
                        //frees the pointer that contains the pointers pointing to each vector
                        free(new_array_vec);
                        return 0;
                    }
                }
                //if vector push back fails
                int check_error = vector_push_back(new_array_vec[index],
                                                   (hash_map->buckets[i]->data[j]));
                if(check_error == 0){
                    for(size_t l = 0; l < new_capacity; l++){
                        vector_free(&(new_array_vec[l]));}
                    free(new_array_vec);
                    return 0;}
            }

        }
    }
    for(size_t i = 0; i < old_capacity; i++)
    {
        if(hash_map->buckets[i] != NULL){
            vector_free(&(hash_map->buckets[i]));
        }
    }
    //update the new data of the new hash table
    free(hash_map->buckets);
    hash_map->buckets = new_array_vec;
    hash_map->capacity = new_capacity;
    return 1;
}


int hashmap_insert(hashmap *hash_map, const pair *in_pair){
    if(hash_map == NULL || in_pair == NULL || hashmap_contains_key(hash_map, in_pair->key)){
        return 0;
    }
    size_t ind = hash_map->hash_func(in_pair->key) & (hash_map->capacity -1);
    vector* hash_place = hash_map->buckets[ind];
    if(hash_place != NULL){
        for (size_t i = 0; i < hash_place->size; i++){
            if (hash_place->elem_cmp_func(hash_place->data[i], in_pair) == 1){
                return 0;
            }
        }
    }
    if(((double)(hash_map->size + 1)/(double)hash_map->capacity)>HASH_MAP_MAX_LOAD_FACTOR){
        if(new_hash_map(hash_map, hash_map->capacity, hash_map->capacity * HASH_MAP_GROWTH_FACTOR) == 0){
            //fails
            return 0;
        }
    }
    //update the new index of the hash table in case the capacity has changed
    ind = hash_map->hash_func(in_pair->key) & (hash_map->capacity -1);
    if (hash_map->buckets[ind] == NULL){
        vector *new_vector = vector_alloc(pair_copy,pair_cmp,pair_free);
        if(new_vector == NULL) {
            return 0;
        }
        hash_map->buckets[ind] = new_vector;
        if (vector_push_back(hash_map->buckets[ind],in_pair) == 0){
            vector_free(&new_vector);
            return 0;
        }
        hash_map->size = hash_map->size + 1;
        return 1;
    }
    if (vector_push_back(hash_map->buckets[ind],in_pair) == 0){
        return 0;
    }
    hash_map->size = hash_map->size + 1;
    return 1;
}
valueT hashmap_at(const hashmap *hash_map, const_keyT key){
    if (hash_map == NULL || key == NULL){
        return NULL;
    }
    size_t ind = (hash_map->hash_func(key)) & (hash_map->capacity - 1);
    if (hash_map->buckets[ind] == NULL){
        return NULL;
    }
    for(size_t i = 0; i < hash_map->buckets[ind]->size; i++){
        pair*my_pair = (pair*)(hash_map->buckets[ind]->data[i]);
        if(my_pair->key_cmp(my_pair->key, key) == 1){
            return my_pair->value;
        }
    }
    return NULL;
}

int hashmap_contains_key(hashmap *hash_map, const_keyT key){
    if (hash_map == NULL || key == NULL){
        return 0;
    }
    size_t ind = hash_map->hash_func(key) & (hash_map->capacity - 1);
    if (hash_map->buckets[ind] == NULL){
        return 0;
    }
    for(size_t i = 0; i < hash_map->buckets[ind]->size; i++){
        pair*my_pair = (pair*)(hash_map->buckets[ind]->data[i]);
        if(my_pair->key_cmp(my_pair->key, key) == 1){
            return 1;
        }
    }
    return 0;
}
int hashmap_contains_value(hashmap *hash_map, const_valueT value){
    if (hash_map == NULL || value == NULL){
        return 0;
    }
    for(size_t i = 0; i < hash_map->capacity; i++){
        if (hash_map->buckets[i] != NULL){
            for(size_t j = 0; j < hash_map->buckets[i]->size; j++){
                pair*my_pair = (pair*)(hash_map->buckets[i]->data[j]);
                if(my_pair->value_cmp(my_pair->value, value) == 1){
                    return 1;
                }
            }
        }
    }
    return 0;
}


int hashmap_erase (hashmap *hash_map, const_keyT key)
{
    if(hash_map == NULL || key == NULL)
    {
        return 0;
    }
    size_t ind = hash_map->hash_func(key) & (hash_map->capacity - 1);
    if(hash_map->buckets[ind] == NULL){
        return 0;
    }
    for(size_t i = 0; i < hash_map->buckets[ind]->size; i++){
        pair* my_pair = (struct pair*)(hash_map->buckets[ind]->data[i]);
        if(my_pair->key_cmp(my_pair->key, key) == 1){
            if(vector_erase(hash_map->buckets[ind], i) == 0)
            {
                return 0;
            }
            if(hash_map->buckets[ind]->size-1 == 0)
            {
                vector_free(&(hash_map->buckets[ind]));
                hash_map->buckets[ind] = NULL;
            }
            if(((double)(hash_map->size - 1) / (double)(hash_map->capacity))
            < HASH_MAP_MIN_LOAD_FACTOR){
                if(new_hash_map(hash_map, hash_map->capacity,
                                hash_map->capacity / HASH_MAP_GROWTH_FACTOR) == 0){
                    return 0;
                }
            }
            hash_map->size = hash_map->size - 1;
            return 1;
        }
    }
    return 0;
}

double hashmap_get_load_factor (const hashmap *hash_map)
{
    if(hash_map == NULL)
    {
        return -1;
    }
    if(hash_map->capacity == 0){
        return -1;
    }
    return ((double)(hash_map->size) / (double)(hash_map->capacity));
}
void hashmap_clear(hashmap *hash_map){
    if(hash_map == NULL){
        return;
    }
    for(size_t i = 0; i < hash_map->capacity;i++){
        if(hash_map->buckets[i] != NULL){
            for(size_t j = hash_map->buckets[i]->size - 1; j > 0; j--){
                struct pair *my_pair = (struct pair*)(hash_map->buckets[i]->data[j]);
                hashmap_erase(hash_map, my_pair->key);
            }
            struct pair *my_pair = (struct pair*)(hash_map->buckets[i]->data[0]);
            hashmap_erase(hash_map, my_pair->key);
        }
    }
}
