#include <glad/glad.h>
#include "renderer.h"
#include "shader.h"

extern void age_internal_shaders_end(void);
extern void age_internal_shaders_begin(void);

void
age_internal_renderer_begin(void *gl_loader) {
  gladLoadGLLoader(gl_loader);
  age_internal_shaders_begin();
}

void
age_internal_renderer_update(void) {
  glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
age_internal_renderer_end(void) {
  age_internal_shaders_end();
}
