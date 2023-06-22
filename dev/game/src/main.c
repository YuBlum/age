#include "keyboard.h"
#include "os.h"
#include "window.h"

void
game_update(void) {
  if (keyboard_get('Q').down) window_force_close();
}

void
game_config(struct winconfig *win) {
  win->name  = "sandbox";
  win->scale = 1.5f;
}
