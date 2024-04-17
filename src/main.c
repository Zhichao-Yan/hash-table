#include "hash_table.h"
#include <stdio.h>

void print_last_name(hash_table *ht,char *first_name)
{
    char *last_name = ht_search(ht,first_name);
    if(last_name != NULL)
    {
        printf("the lastname of %s is %s\n",first_name,last_name);
    }else{
        printf("cannot find %s's lastname!\n",first_name);
    }
    return;
}

int main() {
    hash_table* ht = new_hash_table();
    ht_insert(ht,"hello","world");
    ht_insert(ht,"zhichao","yan");
    ht_insert(ht,"hanxing","qian");
    ht_insert(ht,"yinmeng","hu");
    ht_insert(ht,"yuanzhang","zhu");
    print_last_name(ht,"zhichao");
    ht_delete(ht,"yinmeng");
    print_last_name(ht,"yinmeng");
    ht_update(ht,"zhichao","Liu");
    print_last_name(ht,"zhichao");
    del_hash_table(ht);
}
