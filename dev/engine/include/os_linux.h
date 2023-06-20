#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <dlfcn.h>
#include "os.h"

static i8 abs_path[MAX_PATH + 1];
static b8 init_abs_path = 1;

#define LIB_OPEN(PATH) dlopen(PATH, RTLD_LAZY)
#define LIB_FUNC(LIB, NAME) dlsym(LIB, NAME)
#define LIB_CLOSE(LIB) dlclose(LIB)

void
crash(const i8 *format, ...) {
  i8 msg[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(msg, 1024, format, args);
  va_end(args);
  i8 command[1064];
  snprintf(command, 1064, "zenity --error --text='%s' --title='error'", msg);
  system(command);
  exit(1);
}

void
resource_path_init(void) {
  if (!init_abs_path) return;
  i64 len = readlink("/proc/self/exe", abs_path, MAX_PATH);
  if (len < 0) crash("could not get the executable path: %s\n", strerror(errno));
  abs_path[len] = '\0';
  for (i32 i = len - 1; i >= 0; i--) {
    if (abs_path[i] != '/') continue;
    abs_path[i + 1] = '\0';
    break;
  }
  init_abs_path = 0;
}
