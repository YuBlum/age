#ifdef LINUX
#include "os_linux.h"
#elif WINDOWS
#include "os_windows.h"
#else
#error "Unknown OS target"
#endif

static i8 res_path[MAX_PATH * 2 + 1];
static i8 *res_types[RES_COUNT] = {
  "shaders",
};

const i8 *
age_resource_path(enum restype type, const i8 *name) {
  assert(type < RES_COUNT);
  snprintf(res_path, MAX_PATH * 2, "%s/%s/%s", abs_path, res_types[type], name);
  return res_path;
}
