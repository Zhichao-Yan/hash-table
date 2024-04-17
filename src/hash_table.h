#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define INITIAL_BASE_SIZE 53
#define PRIME1 3
#define PRIME2 5

typedef struct{
    char *key;
    char *val;
}item;

typedef struct{
    size_t base_size;
    size_t size;
    size_t count;
    item** items;
}hash_table;


hash_table* new_hash_table();
void del_hash_table(hash_table *ht);

char *ht_search(hash_table *ht, const char *key);
void ht_insert(hash_table *ht, const char *key, const char *value);
void ht_delete(hash_table *ht, const char *key);
void ht_update(hash_table *ht, const char *key,const char* value);

int next_prime(int x);
bool is_prime(int num);

#endif