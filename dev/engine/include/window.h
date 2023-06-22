#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "types.h"

struct winconfig {
  i8  *name;       /* default: "game" */
  u32  width;      /* default: 640    */
  u32  height;     /* default: 320    */
  f32  scale;      /* default: 1      */
  b8   resizable;  /* default: 0      */
  b8   fullscreen; /* default: 0      */
};

void window_force_close(void);

#endif/*__WINDOW_H__*/
