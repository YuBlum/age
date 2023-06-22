#include <GL/gl.h>
#include "renderer.h"

static struct {
  void (*clear)(u32);
  void (*clear_color)(f32, f32, f32, f32);
} gl;

void
renderer_begin(void *(*gl_loader)(const i8 *)) {
  gl.clear       = gl_loader("glClear");
  gl.clear_color = gl_loader("glClearColor");
}

void
renderer_update(void) {
  gl.clear_color(0.8f, 0.1f, 0.1f, 1.0f);
  gl.clear(GL_COLOR_BUFFER_BIT);
}

void
renderer_end(void) {
}
