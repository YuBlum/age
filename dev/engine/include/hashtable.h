#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "types.h"
#include "simplestr.h"

void *age_hashtable_create(u32 type);
b8    age_hashtable_add(void **hashtable_addr, const struct age_string str, const void *val);
b8    age_hashtable_del(void *hashtable, const struct age_string str);
b8    age_hashtable_get(void *hashtable, const struct age_string str, void *val);
u32   age_hashtable_capacity(void *hashtable);
void  age_hashtable_destroy(void *hashtable);

#endif/*__HASHTABLE_H__*/
