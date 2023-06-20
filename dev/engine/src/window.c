#include <glfw3.h>
#include <stdio.h>
#include "window.h"
#include "game.h"
#include "os.h"

struct vidmode {
  i32 x, y;
};

struct {
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
  void       *(*get_primary_monitor)(void);
  void        (*set_clipboard_string)(void *, const i8 *);
  const i8   *(*get_clipboard_string)(void *);
  const void *(*get_video_mode)(void *);
  void         *lib;
} glfw;

static void *window;
static struct winconfig conf = {
  .name       = "game",
  .width      = 640,
  .height     = 320,
  .scale      = 1,
  .resizable  = 0,
  .fullscreen = 0
};

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
  window = glfw.create_window(conf.width * conf.scale, conf.height * conf.scale, conf.name, NULL, NULL);
  if (!window) {
    const i8 *log;
    glfw.get_error(&log);
    crash("couldn't create window: %s", log);
  }
  glfw.make_context_current(window);
}

static b8
window_run(void) {
  glfw.poll_events();
  glfw.swap_buffers(window);
  return !glfw.window_should_close(window);
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
  while (window_run());
  window_close();
  lib_close(glfw.lib);
}
