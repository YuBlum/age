#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "types.h"

void renderer_begin(void *(*gl_loader)(const i8 *));
void renderer_update(void);
void renderer_end(void);

#endif/*__RENDERER_H__*/
