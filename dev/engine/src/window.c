#include <glfw3.h>
#include <stdio.h>
#include "os.h"
#include "game.h"
#include "utils.h"
#include "window.h"
#include "renderer.h"
#include "keyboard.h"

struct vidmode {
  i32 w, h;
};

static struct {
  i32         (*init)(void);
  i32         (*get_error)(const i8 **);
  void       *(*create_window)(i32, i32, const i8 *, void *, void *);
  void        (*window_hint)(i32, i32);
  i32         (*window_should_close)(void *);
  void        (*set_window_pos)(void *, i32, i32);
  void        (*poll_events)(void);
  void        (*swap_buffers)(void *);
  void        (*make_context_current)(void *);
  void        (*swap_interval)(i32);
  void       *(*get_proc_address)(const i8 *);
  void        (*terminate)(void);
  void        (*set_key_callback)(void *, void *);
  void        (*set_mouse_button_callback)(void *, void *);
  void        (*set_cursor_pos_callback)(void *, void *);
  void        (*set_scroll_callback)(void *, void *);
  void        (*set_window_should_close)(void *, i32);
  void       *(*get_primary_monitor)(void);
  void        (*set_clipboard_string)(void *, const i8 *);
  const i8   *(*get_clipboard_string)(void *);
  const void *(*get_video_mode)(void *);
  void         *lib;
} glfw;

static void *window;
static void *monitor;
static struct winconfig conf = {
  .name       = "game",
  .width      = 640,
  .height     = 320,
  .scale      = 1,
  .resizable  = 0,
  .fullscreen = 0
};

struct internal_key {
  b8 prv;
  b8 cur;
};

extern struct internal_key *keyboard_reference_key(enum keys key);

static void
key_callback(UNUSED void *window, i32 input_key, UNUSED i32 scancode, i32 action, UNUSED i32 mods) {
  enum keys key = KEY_INVALID;
  if (input_key > 0 && input_key < 96) {
    key = input_key;
  } else if (input_key < 256) {
    key = input_key - GLFW_KEY_WORLD_1 + KEY_WORLD_1;
  } else if (input_key <= 301) {
    key = input_key - 256;
  } else if (input_key > 319 && input_key < 330) {
    key = input_key - GLFW_KEY_KP_0 + KEY_PAD_0;
  } else if (input_key < 336) {
    key = input_key - GLFW_KEY_KP_DECIMAL + KEY_PAD_DECIMAL;
  } else if (input_key < 343) {
    key = input_key - GLFW_KEY_KP_EQUAL + KEY_PAD_EQUAL;
  } else if (input_key == 343) {
    key = KEY_LEFT_SUPER;
  } else if (input_key == 344) {
    key = KEY_RIGHT_SHIFT;
  } else if (input_key < 348) {
    key = input_key - GLFW_KEY_RIGHT_CONTROL + KEY_RIGHT_CONTROL;
  }
  struct internal_key *k = keyboard_reference_key(key);
  if (!k) NULL;
  k->prv = k->cur;
  k->cur = action == GLFW_PRESS;
}

static void
load_glfw(void) {
  glfw.lib                       = lib_open(GLFW_NAME);
  glfw.init                      = lib_func(glfw.lib, "glfwInit");
  glfw.get_error                 = lib_func(glfw.lib, "glfwGetError");
  glfw.create_window             = lib_func(glfw.lib, "glfwCreateWindow");
  glfw.window_hint               = lib_func(glfw.lib, "glfwWindowHint");
  glfw.window_should_close       = lib_func(glfw.lib, "glfwWindowShouldClose");
  glfw.set_window_pos            = lib_func(glfw.lib, "glfwSetWindowPos");
  glfw.poll_events               = lib_func(glfw.lib, "glfwPollEvents");
  glfw.swap_buffers              = lib_func(glfw.lib, "glfwSwapBuffers");
  glfw.make_context_current      = lib_func(glfw.lib, "glfwMakeContextCurrent");
  glfw.swap_interval             = lib_func(glfw.lib, "glfwSwapInterval");
  glfw.get_proc_address          = lib_func(glfw.lib, "glfwGetProcAddress");
  glfw.terminate                 = lib_func(glfw.lib, "glfwTerminate");
  glfw.set_key_callback          = lib_func(glfw.lib, "glfwSetKeyCallback");
  glfw.set_mouse_button_callback = lib_func(glfw.lib, "glfwSetMouseButtonCallback");
  glfw.set_cursor_pos_callback   = lib_func(glfw.lib, "glfwSetCursorPosCallback");
  glfw.set_scroll_callback       = lib_func(glfw.lib, "glfwSetScrollCallback");
  glfw.get_primary_monitor       = lib_func(glfw.lib, "glfwGetPrimaryMonitor");
  glfw.set_clipboard_string      = lib_func(glfw.lib, "glfwSetClipboardString");
  glfw.get_clipboard_string      = lib_func(glfw.lib, "glfwGetClipboardString");
  glfw.get_video_mode            = lib_func(glfw.lib, "glfwGetVideoMode");
  glfw.set_window_should_close   = lib_func(glfw.lib, "glfwSetWindowShouldClose");
}

static void
window_open(void) {
  if (!glfw.init()) {
    const i8 *log;
    glfw.get_error(&log);
    crash("couldn't init glfw: %s", log);
  }
  glfw.window_hint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfw.window_hint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfw.window_hint(GLFW_RESIZABLE, conf.resizable);
  u32 window_w = conf.width  * conf.scale;
  u32 window_h = conf.height * conf.scale;
  window = glfw.create_window(window_w, window_h, conf.name, NULL, NULL);
  if (!window) {
    const i8 *log;
    glfw.get_error(&log);
    crash("couldn't create window: %s", log);
  }
  monitor = glfw.get_primary_monitor();
  if (!monitor) {
    const i8 *log;
    glfw.get_error(&log);
    crash("couldn't get monitor: %s", log);
  }
  const struct vidmode *vidmode = glfw.get_video_mode(monitor);
  glfw.set_window_pos(window, (vidmode->w << 1) - (window_w << 1), (vidmode->h << 1) - (window_h << 1));
  glfw.make_context_current(window);
  /* setup callbacks */
  glfw.set_key_callback(window, key_callback);
}

static b8
window_run(void) {
  game_update();
  renderer_update();
  glfw.poll_events();
  glfw.swap_buffers(window);
  return !glfw.window_should_close(window);
}

void
window_force_close(void) {
  glfw.set_window_should_close(window, 1);
}

static void
window_close(void) {
  glfw.terminate();
}

int
main(void) {
  load_glfw();
  game_config(&conf);
  window_open();
  renderer_begin(glfw.get_proc_address);
  while (window_run());
  renderer_end();
  window_close();
  lib_close(glfw.lib);
}
