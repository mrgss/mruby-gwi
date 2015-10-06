/*
 * Copyright 2015 MRGSS developers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gwi.h"
#include "radixtree.h"

static int gwi_initialized = 0;
static gwi_Color gwi_bg;
extern rt_tree  *gwi_events;

#ifdef _WIN32
#include "gwi/win32/window.h"
#endif

void
gwi_init(const char *title, size_t width, size_t height)
{
  if (gwi_initialized) return;
  gwi_initialized = 1;
  gwi_bg.red = gwi_bg.green = gwi_bg.blue = gwi_bg.alpha = 255;
  gwi_open_window(title, width, height);
  gwi_events = rt_tree_malloc(255, gwi_free, gwi_alloc, realloc, gwi_free);
  assert(gwi_events);
}

void
gwi_end(void)
{
  if (!gwi_initialized) return;
  gwi_close_window();
  gwi_initialized = 0;
  rt_tree_free(gwi_events);
  gwi_events = NULL;
}

void
gwi_main_loop(void *context, gwi_loop_callback callback)
{
  if (!gwi_initialized) return;
  while (gwi_update_window(context, callback));
}

void
gwi_set_background(gwi_Color *color)
{
  gwi_bg = *color;
  gwi_refresh();
}

void
gwi_get_background(gwi_Color *color)
{
  *color = gwi_bg;
}
