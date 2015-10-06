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

rt_tree  *gwi_events = NULL;

struct gwi_event_node
{
  void              *context;
  gwi_event_callback callback;
};


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
