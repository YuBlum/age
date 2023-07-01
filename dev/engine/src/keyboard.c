#include <glfw3.h>
#include <string.h>
#include "keyboard.h"

struct age_internal_key {
  b8 prv;
  b8 cur;
};

static struct age_internal_key keys[AGE_KEY_COUNT];

void
age_keyboard_refresh(void) {
  memset(keys, 0, AGE_KEY_COUNT * sizeof (struct age_internal_key));
}

struct age_internal_key *
age_internal_keyboard_reference_key(enum age_keys key) {
  if (key > AGE_KEY_COUNT) return NULL;
  return keys + key;
}

struct age_key
age_keyboard_get(enum age_keys key) {
  return (struct age_key) {
    .up    = !keys[key].cur,
    .down  =  keys[key].cur,
    .click =  keys[key].cur && !keys[key].prv
  };
}
