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

void
age_crash(const i8 *format, ...) {
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
age_internal_resource_path_init(void) {
  i64 len = readlink("/proc/self/exe", abs_path, MAX_PATH);
  if (len < 0) age_crash("could not get the executable path: %s\n", strerror(errno));
  abs_path[len] = '\0';
  for (i32 i = len - 1; i >= 0; i--) {
    if (abs_path[i] != '/') continue;
    abs_path[i + 1] = '\0';
    break;
  }
}
