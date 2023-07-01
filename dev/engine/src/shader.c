#include <glad/glad.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "shader.h"
#include "hashtable.h"

static u32 *shaders;

static u32
age_internal_file_to_shader(struct age_string name, struct age_string shader_type_str, u32 shader_type) {
  i8 shader_name[MAX_PATH + 1];
  snprintf(shader_name, MAX_PATH, "%.*s/%.*s.glsl", AGE_STRFORMAT(name), AGE_STRFORMAT(shader_type_str));
  const i8 *shader_path = age_resource_path(RES_SHADER, shader_name);
  FILE *shader_file = fopen(shader_path, "r");
  if (!shader_file) {
    age_crash("couldn't open file '%s': %s", shader_path, strerror(errno));
  }
  fseek(shader_file, 0, SEEK_END);
  i32 shader_file_size = ftell(shader_file);
  i8 *shader_source = malloc(shader_file_size + 1);
  shader_source[shader_file_size] = '\0';
  rewind(shader_file);
  (void)fread(shader_source, 1, shader_file_size, shader_file);
  fclose(shader_file);
  u32 shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, (const i8 **)&shader_source, &shader_file_size);
  glCompileShader(shader);
  i32 status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    i8 infolog[1028];
    glGetShaderInfoLog(shader, 1028, NULL, infolog);
    age_crash("compilation failed: %.*s shader of %.*s: %s", AGE_STRFORMAT(shader_type_str), AGE_STRFORMAT(name), infolog);
  }
  return shader;
}

void
age_internal_shaders_begin(void) {
  shaders = age_hashtable_create(sizeof (u32));
}

void
age_shader_load(struct age_string name) {
  u32 shader   = glCreateProgram();
  u32 vertex   = age_internal_file_to_shader(name, AGE_STR("vertex"), GL_VERTEX_SHADER);
  u32 fragment = age_internal_file_to_shader(name, AGE_STR("fragment"), GL_FRAGMENT_SHADER);
  glAttachShader(shader, vertex);
  glAttachShader(shader, fragment);
  glLinkProgram(shader);
  i32 status;
  glGetProgramiv(shader, GL_LINK_STATUS, &status);
  if (!status) {
    i8 infolog[1028];
    glGetProgramInfoLog(shader, 1028, NULL, infolog);
    age_crash("linking error on shader %.*s: %s", AGE_STRFORMAT(name), infolog);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (!age_hashtable_add((void **)&shaders, name, &shader)) {
    age_crash("trying to load shader '.*s' twice", AGE_STRFORMAT(name));
  }
}

void
age_shader_unload(struct age_string name) {
  u32 shader;
  if (!age_hashtable_get(shaders, name, &shader)) {
    age_crash("trying to unload shader '.*s', but it's not loaded", AGE_STRFORMAT(name));
  }
  glDeleteShader(shader);
  assert(age_hashtable_del(shaders, name) && "unreachble");
}

void
age_shader_use(struct age_string name) {
  u32 shader;
  if (!age_hashtable_get(shaders, name, &shader)) {
    age_crash("trying to use shader '.*s', but it's not loaded", AGE_STRFORMAT(name));
  }
  glUseProgram(shader);
}

void
age_internal_shaders_end(void) {
  age_hashtable_destroy(shaders);
}
