
#include "hash_table.h"

static item DELETED_ITEM = {NULL, NULL};

/* create an item */
static item *new_item(const char *k, const char *v)
{
    item *i = malloc(sizeof(item));
    // strdup返回一个指向新分配内存的指针，该内存中存储了原始字符串的副本
    i->key = strdup(k);
    i->val = strdup(v);
    return i;
}

/* delete an item and free it */
static void del_item(item *i)
{
    if(i == &DELETED_ITEM)
        return;
    free(i->key);
    free(i->val);
    free(i);
    return;
}

/* hash function */
static int hash(const char *s, const int prime, const int buckets)
{
    long hash = 0;
    const int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        hash += (long)pow(prime, len - (i + 1)) * s[i];
    }
    hash = hash % buckets;
    return (int)hash;
}

/* get hash using double hashing  */
static int get_hash(const char *s, const int buckets, const int collision)
{
    const int hash1 = hash(s, PRIME1, buckets);
    const int hash2 = hash(s, PRIME2, buckets);
    return (hash1 + (collision * (hash2 + 1))) % buckets;
}

/* create a hash table with base_size */
static hash_table* hash_table_sized(const int base_size) 
{
    hash_table* ht = malloc(sizeof(hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(item*));
    return ht;
}

/* resize hash table to base_size */
static void hash_table_resize(hash_table* ht, const int base_size) 
{
    // don't reduce the size of the hash table below its minimum
    if (base_size < INITIAL_BASE_SIZE) {
        return;
    }
    hash_table* new_ht = hash_table_sized(base_size);
    for (int i = 0; i < ht->size; i++) 
    {
        item* it = ht->items[i];
        if (it != NULL && it != &DELETED_ITEM) {
            ht_insert(new_ht, it->key, it->val);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;
    del_hash_table(new_ht);
}

/* resize up the hash table */
static void resize_up(hash_table* ht) 
{
    const int new_size = ht->base_size * 2;
    hash_table_resize(ht, new_size);
}

/* resize down the hash table */
static void resize_down(hash_table* ht) 
{
    const int new_size = ht->base_size / 2;
    hash_table_resize(ht, new_size);
}


/* create a hash table with initalized base size of 53*/
hash_table *new_hash_table()
{
    return hash_table_sized(INITIAL_BASE_SIZE);  
}

/* delete hash_table */
void del_hash_table(hash_table *ht)
{
    for (size_t i = 0; i < ht->size; ++i)
    {
        item *it = ht->items[i];
        if (it != NULL)
        {
            del_item(it);
        }
    }
    free(ht->items);
    free(ht);
}

/* insert a pair of key and value */
void ht_insert(hash_table *ht, const char *key, const char *value)
{
    // above the predefined limits of 0.7
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        resize_up(ht);
    }
    item *it = new_item(key, value);
    int index = get_hash(it->key, ht->size, 0); // get index
    int i = 1;                                  // collision times
    item *cur = ht->items[index];               // current item at index
    while (cur != NULL && cur != &DELETED_ITEM) // collision happened and handle collision
    {
        index = get_hash(it->key, ht->size, i); // open-addressed and double hash
        cur = ht->items[index];
        i++;
    }
    // if cur == NULL or cur is pointed to DELETED_ITEM,we can insert an item at this location
    ht->items[index] = it; // finally get an empty slot
    ht->count++;           // items number
    return;
}

/* seach item with key */
char *ht_search(hash_table *ht, const char *key)
{
    int index = get_hash(key, ht->size, 0);
    item *it = ht->items[index];
    int i = 1;
    while (it != NULL)
    {
        if (it != &DELETED_ITEM)
        {
            if (strcmp(it->key, key) == 0) // check whether the item's key matches the key
            {
                return it->val; // return key's value
            }        
        }
        index = get_hash(key, ht->size, i); // handle collision and rehash
        it = ht->items[index];
        i++;
    }
    return NULL;
}

/* update the key and value pair */
void ht_update(hash_table *ht, const char *key,const char* value)
{
    /* delete the previous item and insert the new item at its location. 
    * if both items have same key,ht->count is still the same
    */
    int index = get_hash(key, ht->size, 0);
    item *cur = ht->items[index];
    int i = 1;
    while (cur != NULL)
    {
        if (cur != &DELETED_ITEM)
        {
            if (strcmp(cur->key, key) == 0)
            {
                strcpy(cur->val,value);
                return;
            }
        }
        index = get_hash(key, ht->size, i);
        cur = ht->items[index];
        i++;
    }
    return;
}

/* delete the item with key */
void ht_delete(hash_table *ht, const char *key)
{
    // below predefined limits of 0.1
    const int load = ht->count * 100 / ht->size;
    if (load < 10) 
    {
        resize_down(ht);
    }
    int index = get_hash(key, ht->size, 0);
    item *it = ht->items[index];
    int i = 1;
    while (it != NULL)
    {
        if (it != &DELETED_ITEM)
        {
            if (strcmp(it->key, key) == 0)
            {
                del_item(it);
                ht->items[index] = &DELETED_ITEM;
                ht->count--;
                return;
            }
        }
        index = get_hash(key, ht->size, i);
        it = ht->items[index];
        i++;
    }
    return;
}

/* check if num is a prime number */
bool is_prime(int num) 
{
    if(num <= 1)
        return false;
    for(int i = 2; i * i <= num; ++i)
    {
        if(num%i == 0)
            return false;
    }
    return true;
}

/* find the prime number since x */
int next_prime(int x) 
{
    while (is_prime(x) != true) 
    {
        x++;
    }
    return x;
}