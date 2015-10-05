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
#include "gwi/handle.h"
#include "radixtree.h"

struct gwi_event_node
{
  void              *context;
  gwi_event_callback callback;
};

static int gwi_initialized = 0;
static rt_tree  *gwi_events;
static gwi_Color gwi_bg;

static gwi_free_fn on_free = NULL;
static gwi_alloc_fn on_alloc = NULL;
static void  *gwi_alloc_ctx = NULL;
static void  *gwi_free_ctx  = NULL;

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
}

void
gwi_main_loop(void *context, gwi_loop_callback callback)
{
  if (!gwi_initialized) return;
  while (gwi_update_window(context, callback));
}

void
gwi_on(const char *name, void *context, gwi_event_callback callback)
{
  struct gwi_event_node *handle;
  handle = malloc(sizeof *handle);
  assert(handle);
  handle->context = context;
  handle->callback   = callback;
  assert(rt_tree_set(gwi_events, (const unsigned char *)name, strlen(name), handle));
}

void
gwi_off(const char *name)
{
  struct gwi_event_node *handle;
  handle = rt_tree_get(gwi_events, (const unsigned char *)name, strlen(name));
  if (handle) free(handle);
}

void
gwi_fire(const char *name, gwi_Event *event)
{
  struct gwi_event_node *handle;
  handle = rt_tree_get(gwi_events, (const unsigned char *)name, strlen(name));
  if (handle) handle->callback(handle->context, event);
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

void
gwi_set_alloc_fn(void *ctx, gwi_alloc_fn fn )
{
  on_alloc = fn;
  gwi_alloc_ctx = ctx;
}

void
gwi_set_free_fn(void *ctx, gwi_free_fn fn )
{
  on_free = fn;
  gwi_free_ctx = ctx;
}

void *
gwi_alloc(size_t size)
{
  if (on_alloc)
    return on_alloc(gwi_alloc_ctx, size);
  return malloc(size);
}

void
gwi_free(void *ptr)
{
  if (on_free)
    on_free(gwi_free_ctx, ptr);
  free(ptr);
}

void
gwi_free_handle(gwi_Handle *handle)
{
  gwi_free(handle);
}
