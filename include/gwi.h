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
#ifndef MRUBY_GWI_H
#define MRUBY_GWI_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gwi_Handle gwi_Handle;
typedef struct gwi_Event  gwi_Event;
typedef struct gwi_Color  gwi_Color;
typedef struct gwi_Size   gwi_Size;

typedef void  (*gwi_loop_callback)(void *);
typedef void  (*gwi_event_callback)(void *, gwi_Event *);
typedef void *(*gwi_alloc_fn)(void *, size_t);
typedef void  (*gwi_free_fn)(void *, void *);

struct gwi_Event
{
  const char *name;

};

struct gwi_Color
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
};

struct gwi_Size
{
  unsigned short top;
  unsigned short bottom;
  unsigned short left;
  unsigned short right;
};


void
gwi_init(const char *title, size_t width, size_t height);

void
gwi_end(void);

void
gwi_main_loop(void *context, gwi_loop_callback callback);

void
gwi_message(const char *title, const char *msg);

void
gwi_error(const char *title, const char *msg);

void
gwi_info(const char *title, const char *msg);

void
gwi_warning(const char *title, const char *msg);

int
gwi_ask(const char *title, const char *msg);

int
gwi_accept(const char *title, const char *msg);

void
gwi_on(const char *name, void *context, gwi_event_callback event);

void
gwi_off(const char *name);

void
gwi_fire(const char *name, gwi_Event *event);

char *
gwi_file_open_dialog(void);

char *
gwi_file_save_dialog(void);

void
gwi_set_background(gwi_Color *color);

void
gwi_get_background(gwi_Color *color);

void
gwi_refresh(void);

void
gwi_set_alloc_fn(void *ctx, gwi_alloc_fn fn );

void
gwi_set_free_fn(void *ctx, gwi_free_fn fn );

void *
gwi_alloc(size_t size);

void
gwi_free(void *ptr);

void
gwi_free_handle(gwi_Handle *handle);

int
gwi_Panel(gwi_Handle *p);

void
gwi_handle_on(gwi_Handle *h, const char *name, void *context, gwi_event_callback event);

void
gwi_handle_off(gwi_Handle *h, const char *name);

void
gwi_handle_fire(gwi_Handle *h, const char *name, gwi_Event *evt);

#ifdef __cplusplus
}
#endif

#endif /* MRUBY_GWI_H */
