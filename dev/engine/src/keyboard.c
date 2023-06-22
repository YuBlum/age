#include <glfw3.h>
#include <string.h>
#include "keyboard.h"

struct internal_key {
  b8 prv;
  b8 cur;
};

static struct internal_key keys[KEY_COUNT];

void
keyboard_refresh(void) {
  memset(keys, 0, KEY_COUNT * sizeof (struct internal_key));
}

struct internal_key *
keyboard_reference_key(enum keys key) {
  if (key > KEY_COUNT) return NULL;
  return keys + key;
}

struct key
keyboard_get(enum keys key) {
  return (struct key) {
    .up    = !keys[key].cur,
    .down  =  keys[key].cur,
    .click =  keys[key].cur && !keys[key].prv
  };
}
