#ifndef __SHADER_H__
#define __SHADER_H__

#include "simplestr.h"
#include "renderer.h"
#include "types.h"

void age_shader_load(struct age_string name);
void age_shader_unload(struct age_string name);
void age_shader_use(struct age_string name);

#endif/*__SHADER_H__*/
