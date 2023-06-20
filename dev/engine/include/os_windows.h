#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include "os.h"

static i8 abs_path[MAX_PATH + 1];
static b8 init_abs_path = 1;

#define LIB_OPEN(PATH) LoadLibrary(PATH)
#define LIB_FUNC(LIB, NAME) GetProcAddress(LIB, NAME)
#define LIB_CLOSE(LIB) FreeLibrary(LIB)

void
crash(const i8 *format, ...) {
  i8 msg[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(msg, 1024, format, args);
  va_end(args);
  MessageBox(NULL, msg, "error", MB_ICONERROR | MB_OK);
}

static void
resource_path_init(void) {
  if (!init_abs_path) return;
  i32 len = GetModuleFileName(NULL, abs_path, MAX_PATH);
  abs_path[len] = '\0';
  for (i32 i = len - 1; i >= 0; i--) {
    if (abs_path[i] != '\\') continue;
    abs_path[i + 1] = '\0';
    break;
  }
  init_abs_path = 0;
}
