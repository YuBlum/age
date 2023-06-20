#ifndef __OS_H__
#define __OS_H__

#include "types.h"

#ifdef LINUX
#  define GLFW_NAME "libglfw.so"
#else
#  define GLFW_NAME "glfw3.dll"
#endif

#undef  MAX_PATH
#define MAX_PATH 512

enum restype {
  RES_LIB = 0,
  RES_COUNT
};

void      crash(const i8 *format, ...);
const i8 *resource_path(enum restype type, const i8 *name);
void     *lib_open(const i8 *lib_name);
void     *lib_func(void *lib, const i8 *func_name);
void      lib_close(void *lib);

#endif/*__OS_H__*/
