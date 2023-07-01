#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "types.h"

struct age_conf {
  i8  *name;       /* default: "game" */
  u32  width;      /* default: 640    */
  u32  height;     /* default: 320    */
  f32  scale;      /* default: 1      */
  b8   resizable;  /* default: 0      */
  b8   fullscreen; /* default: 0      */
};

void age_window_close(void);

#endif/*__WINDOW_H__*/
