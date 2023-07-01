#include <string.h>
#include "simplestr.h"
#include "utils.h"

b8
age_string_equal(const struct age_string *str1, const struct age_string *str2) {
  return strncmp(str1->buf, str2->buf, MAX(str1->siz, str2->siz)) == 0;
}
