#include <glfw3.h>
#include <stdio.h>
#include "os.h"
#include "utils.h"
#include "window.h"
#include "renderer.h"
#include "keyboard.h"
#include "simplestr.h"

static GLFWwindow  *window;
static GLFWmonitor *monitor;
static struct age_conf conf = {
  .name       = "game",
  .width      = 640,
  .height     = 320,
  .scale      = 1,
  .resizable  = 0,
  .fullscreen = 0
};

struct age_internal_key {
  b8 prv;
  b8 cur;
};

extern struct age_internal_key *age_internal_keyboard_reference_key(enum age_keys key);
extern void age_internal_renderer_begin(void *gl_loader);
extern void age_internal_renderer_update(void);
extern void age_internal_renderer_end(void);
extern void age_internal_resource_path_init(void);

extern void on_conf(struct age_conf *conf);
extern void on_load(void);
extern void on_loop(f64 delta_time);
extern void on_exit(void);

static void
age_internal_key_callback(UNUSED GLFWwindow *window, i32 input_key, UNUSED i32 scancode, i32 action, UNUSED i32 mods) {
  enum age_keys key = AGE_KEY_INVALID;
  if (input_key > 0 && input_key < 96) {
    key = input_key;
  } else if (input_key < 256) {
    key = input_key - GLFW_KEY_WORLD_1 + AGE_WORLD_1;
  } else if (input_key <= 301) {
    key = input_key - 256;
  } else if (input_key > 319 && input_key < 330) {
    key = input_key - GLFW_KEY_KP_0 + AGE_PAD_0;
  } else if (input_key < 336) {
    key = input_key - GLFW_KEY_KP_DECIMAL + AGE_PAD_DECIMAL;
  } else if (input_key < 343) {
    key = input_key - GLFW_KEY_KP_EQUAL + AGE_PAD_EQUAL;
  } else if (input_key == 343) {
    key = AGE_LEFT_SUPER;
  } else if (input_key == 344) {
    key = AGE_RIGHT_SHIFT;
  } else if (input_key < 348) {
    key = input_key - GLFW_KEY_RIGHT_CONTROL + AGE_RIGHT_CONTROL;
  }
  struct age_internal_key *k = age_internal_keyboard_reference_key(key);
  if (!k) NULL;
  k->cur = action != GLFW_RELEASE;
}

static void
age_internal_window_open(void) {
  if (!glfwInit()) {
    const i8 *log;
    glfwGetError(&log);
    age_crash("couldn't init glfw: %s", log);
  }
  age_internal_resource_path_init();
  on_conf(&conf);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE, conf.resizable);
  u32 window_w = conf.width  * conf.scale;
  u32 window_h = conf.height * conf.scale;
  window = glfwCreateWindow(window_w, window_h, conf.name, NULL, NULL);
  if (!window) {
    const i8 *log;
    glfwGetError(&log);
    age_crash("couldn't create window: %s", log);
  }
  monitor = glfwGetPrimaryMonitor();
  if (!monitor) {
    const i8 *log;
    glfwGetError(&log);
    age_crash("couldn't get monitor: %s", log);
  }
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
  glfwSetWindowPos(window, (vidmode->width << 1) - (window_w << 1), (vidmode->height << 1) - (window_h << 1));
  glfwMakeContextCurrent(window);
  /* setup callbacks */
  glfwSetKeyCallback(window, age_internal_key_callback);
  /* renderer setup */
  age_internal_renderer_begin(glfwGetProcAddress);
  on_load();
}

static b8
age_internal_window_run(void) {
  glfwPollEvents();
  /* logic */
  on_loop(1.0/60.0);
  /* render */
  age_internal_renderer_update();
  glfwSwapBuffers(window);
  /* input */
  for (u32 i = 0; i < AGE_KEY_COUNT; i++) {
    struct age_internal_key *k = age_internal_keyboard_reference_key(i);
    k->prv = k->cur;
  }
  return !glfwWindowShouldClose(window);
}

static void
age_internal_window_close(void) {
  glfwTerminate();
}

void
age_window_close(void) {
  on_exit();
  age_internal_renderer_end();
  glfwSetWindowShouldClose(window, 1);
}

int
main(void) {
  age_internal_window_open();
  while (age_internal_window_run());
  age_internal_window_close();
}
