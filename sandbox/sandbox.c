#include <stdio.h>
#include <age/window.h>
#include <age/keyboard.h>

void
on_conf(struct age_conf *conf) {
  conf->name  = "sandbox";
  conf->scale = 1.3;
}

void
on_load(void) {
}

void
on_loop(void) {
  if (age_keyboard_get('Q').click) age_window_close();
  if (age_keyboard_get('Z').click) printf("Z!\n");
}

void
on_exit(void) {
}
