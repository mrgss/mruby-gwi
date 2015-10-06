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

static gwi_free_fn on_free = NULL;
static gwi_alloc_fn on_alloc = NULL;
static void  *gwi_alloc_ctx = NULL;
static void  *gwi_free_ctx  = NULL;

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
  void *result;
  if (on_alloc)
    result = on_alloc(gwi_alloc_ctx, size);
  else
    result = malloc(size);
  assert(result);
  return result;
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
