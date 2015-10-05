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
#include "art.h"

struct gwi_event_node
{
  void              *context;
  gwi_event_callback callback;
};

static int gwi_initialized = 0;
static art_tree gwi_events;


#ifdef _WIN32
#include "gwi/win32/window.h"
#endif

void
gwi_init(const char *title, size_t width, size_t height)
{
  if (gwi_initialized) return;
  gwi_open_window(title, width, height);
  assert(!art_tree_init(&gwi_events));
  gwi_initialized = 1;
}

void
gwi_end(void)
{
  if (!gwi_initialized) return;
  gwi_close_window();
  gwi_initialized = 0;
  assert(!art_tree_destroy(&gwi_events));
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
  handle = art_insert(&gwi_events, (const unsigned char *)name, (int)strlen(name), handle);
  if (handle) free(handle);
}

void
gwi_off(const char *name)
{
  struct gwi_event_node *handle;
  handle = art_delete(&gwi_events, (const unsigned char *)name, (int)strlen(name));
  if (handle) free(handle);
}

void
gwi_fire(const char *name, gwi_Event *event)
{
  struct gwi_event_node *handle;
  handle = art_search(&gwi_events, (const unsigned char *)name, (int)strlen(name));
  if (handle) handle->callback(handle->context, event);
}
