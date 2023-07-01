#ifndef __SIMPLESTR_H__
#define __SIMPLESTR_H__

#include "types.h"
#include <string.h>

struct age_string {
  i8 *buf;
  u32 siz;
};
#define AGE_STR(S) ((struct age_string) { S, strlen(S) })
#define AGE_STRFORMAT(S) S.siz, S.buf

b8 age_string_equal(const struct age_string *str1, const struct age_string *str2);

#endif/*__SIMPLESTR_H__*/
