#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include "os.h"
#include "utils.h"

#define INIT_CAP 17

struct age_hashtable {
  u32 cap;
  u32 typ;
  u32 siz;
};

/* http://www.cse.yorku.ca/~oz/hash.html */
static u64
age_internal_hash(const struct age_string *str) {
  u64 hash = 5381;
  for (u32 i = 0; i < str->siz; i++)
    hash = ((hash << 5) + hash) + str->buf[i]; /* hash * 33 + c */
  return hash;
}

void *
age_hashtable_create(u32 type) {
  struct age_hashtable *ht = malloc(sizeof (struct age_hashtable) + INIT_CAP * (type + sizeof (struct age_string)));
  if (!ht) return NULL;
  ht->cap = INIT_CAP;
  ht->typ = type;
  ht->siz = 0;
  void *data = (void *)(ht + 1);
  memset(data + (ht->typ * ht->cap), 0, sizeof (struct age_string) * ht->cap);
  return ht;
}

b8
age_hashtable_add(void **hashtable_addr, const struct age_string str, const void *val) {
  struct age_hashtable *ht = *hashtable_addr;
  /* rehash */
  f32 load_factor = (f32)ht->siz / (f32)ht->cap;
  if (load_factor > 0.5f) {
    struct age_hashtable *old_ht = ht;
    u8 *old_data = (u8 *)(old_ht + 1);
    struct age_string *old_strs = (void *)(old_data + (old_ht->typ * old_ht->cap));
    u32 new_cap = ht->cap * 2;
    ht = malloc(sizeof (struct age_hashtable) + new_cap * (sizeof (struct age_string) + old_ht->typ));
    ht->cap = new_cap;
    ht->siz = old_ht->siz;
    ht->typ = old_ht->typ;
    u8 *data = (u8 *)(ht + 1);
    struct age_string *strs = (void *)(data + (ht->typ * ht->cap));
    memset(strs, 0, sizeof (struct age_string) * ht->cap);
    for (u32 j = 0; j < old_ht->cap; j++) {
      if (!old_strs[j].buf) continue;
      u32 index = age_internal_hash(old_strs + j) % ht->cap;
      for (u32 i = index; strs[index].buf; i++) {
        assert(!age_string_equal(&str, strs + index));
        index = i % ht->cap;
      }
      strs[index] = old_strs[j];
      memcpy(data + index * ht->typ, old_data + j * ht->typ, ht->typ);
    }
    free(old_ht);
    *hashtable_addr = ht;
  }
  /* add new item */
  u8 *data = (u8 *)(ht + 1);
  struct age_string *strs = (void *)(data + (ht->typ * ht->cap));
  u32 index = age_internal_hash(&str) % ht->cap;
  for (u32 i = index; strs[index].buf; i++) {
    if (age_string_equal(&str, strs + index)) return 0;
    index = i % ht->cap;
  }
  strs[index].buf = malloc(str.siz);
  strncpy(strs[index].buf, str.buf, str.siz);
  strs[index].siz = str.siz;
  memcpy(data + (index * ht->typ), val, ht->typ);
  ht->siz++;
  return 1;
}

b8
age_hashtable_del(void *hashtable, const struct age_string str) {
  struct age_hashtable *ht = hashtable;
  u8 *data = (u8 *)(ht + 1);
  struct age_string *strs = (void *)(data + (ht->typ * ht->cap));
  u32 index = age_internal_hash(&str) % ht->cap;
  for (u32 i = index; strs[index].buf; i++) {
    if (age_string_equal(&str, strs + index)) {
      free(strs[index].buf);
      strs[index].buf = NULL;
      return 1;
    }
    index = i % ht->cap;
  }
  return 0;
}

b8
age_hashtable_get(void *hashtable, const struct age_string str, void *val) {
  struct age_hashtable *ht = hashtable;
  u8 *data = (u8 *)(ht + 1);
  struct age_string *strs = (void *)(data + (ht->typ * ht->cap));
  u32 index = age_internal_hash(&str) % ht->cap;
  for (u32 i = index; strs[index].buf; i++) {
    if (age_string_equal(&str, strs + index)) {
      if (val) memcpy(val, data + index * ht->typ, ht->typ);
      return 1;
    }
    index = i % ht->cap;
  }
  return 0;
}

u32
age_hashtable_capacity(void *hashtable) {
  return ((struct age_hashtable *)hashtable)->cap;
}

void 
age_hashtable_destroy(void *hashtable) {
  struct age_hashtable *ht = hashtable;
  struct age_string *strs = ((void *)(ht + 1)) + (ht->typ * ht->cap);
  for (u32 i = 0; i < ht->cap; i++) {
    if (!strs[i].buf) continue;
    free(strs[i].buf);
  }
  free(ht);
}
