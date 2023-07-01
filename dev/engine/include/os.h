#ifndef __OS_H__
#define __OS_H__

#include "types.h"

#undef  MAX_PATH
#define MAX_PATH 512

enum restype {
  RES_SHADER,
  RES_COUNT
};

void      age_crash(const i8 *format, ...);
const i8 *age_resource_path(enum restype type, const i8 *name);

#endif/*__OS_H__*/
