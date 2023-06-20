#ifdef LINUX
#include "os_linux.h"
#elif WINDOWS
#include "os_windows.h"
#else
#error "Unknown OS target"
#endif

static i8 res_path[MAX_PATH * 2 + 1];
static i8 *res_types[RES_COUNT] = {
  "libs",
};

const i8 *
resource_path(enum restype type, const i8 *name) {
  resource_path_init();
  assert(RES_LIB < RES_COUNT);
  snprintf(res_path, MAX_PATH * 2, "%s/%s/%s", abs_path, res_types[type], name);
  return res_path;
}

void *
lib_open(const i8 *lib_name) {
  const i8 *path = resource_path(RES_LIB, lib_name);
  void *lib = LIB_OPEN(path);
  if (!lib) crash("could not load '%s'\n", path);
  return lib;
}

void *
lib_func(void *lib, const i8 *func_name) {
  void *func = LIB_FUNC(lib, func_name);
  if (!func) crash("could not get function '%s'", func_name);
  return func;
}

void
lib_close(void *lib) {
  LIB_CLOSE(lib);
}
