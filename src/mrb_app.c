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
#include <stdlib.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/class.h>
#include "gwi.h"

struct mrb_gwi_handle
{
  struct RClass *GWI;
  mrb_state     *mrb;
  mrb_value      block;
};

static void *
mrb_gwi_malloc(void *mrb, size_t size)
{
  return mrb_malloc(mrb, size);
}

static struct RClass *GWI_CLASS;
static struct mrb_gwi_handle handle;

static void
mrb_gwi_update(void *ptr)
{
  struct mrb_gwi_handle *h = ptr;
  if (mrb_respond_to(h->mrb, h->block, mrb_intern_lit(h->mrb, "call"))) {
    mrb_funcall(h->mrb, h->block, "call", 0);
  }
}

static void
mrb_gwi_handle_event(void *ptr, gwi_Event *event)
{
  mrb_value hash, block;
  struct mrb_gwi_handle *h = ptr;
  hash = mrb_mod_cv_get(h->mrb, GWI_CLASS, mrb_intern_lit(h->mrb, "events"));
  block = mrb_hash_get(h->mrb, hash, mrb_str_new_cstr(h->mrb, event->name));
  if (mrb_respond_to(h->mrb, block, mrb_intern_lit(h->mrb, "call")))
    mrb_funcall(h->mrb, block, "call", 1, event);
}

static mrb_value
mrb_gwi_open(mrb_state* mrb, mrb_value self)
{
  char *t;
  mrb_int w, h;
  mrb_get_args(mrb, "zii", &t, &w, &h);
  gwi_init(t, w, h);
  gwi_set_alloc_fn(mrb, mrb_gwi_malloc);
  mrb_mod_cv_set(mrb, GWI_CLASS, mrb_intern_lit(mrb, "events"), mrb_hash_new(mrb));
  return self;
}

static mrb_value
mrb_gwi_close(mrb_state* mrb, mrb_value self)
{
  mrb_mod_cv_set(mrb, GWI_CLASS, mrb_intern_lit(mrb, "events"), mrb_hash_new(mrb));
  gwi_end();
  return self;
}

static mrb_value
mrb_gwi_loop(mrb_state* mrb, mrb_value self)
{
  handle.mrb = mrb;
  mrb_get_args(mrb, "&", &(handle.block));
  mrb_mod_cv_set(mrb, GWI_CLASS, mrb_intern_lit(mrb, "block_value"), handle.block);
  gwi_main_loop(&handle, mrb_gwi_update);
  return self;
}

static mrb_value
mrb_gwi_on(mrb_state* mrb, mrb_value self)
{
  mrb_value hash, name, block;
  handle.GWI = GWI_CLASS;
  mrb_get_args(mrb, "S&", &name, &block);
  hash = mrb_mod_cv_get(mrb, GWI_CLASS, mrb_intern_lit(mrb, "events"));
  mrb_hash_set(mrb, hash, name, block);
  gwi_on(mrb_string_value_ptr(mrb, name), &handle, mrb_gwi_handle_event);
  return self;
}

static mrb_value
mrb_gwi_fire(mrb_state* mrb, mrb_value self)
{
  mrb_value hash, name, event, block;
  mrb_get_args(mrb, "So", &name, &event);
  hash = mrb_mod_cv_get(mrb, GWI_CLASS, mrb_intern_lit(mrb, "events"));
  block = mrb_hash_get(mrb, hash, name);
  if (mrb_respond_to(mrb, block, mrb_intern_lit(mrb, "call")))
    mrb_funcall(mrb, block, "call", 1, event);
  return self;
}


static mrb_value
mrb_gwi_off(mrb_state* mrb, mrb_value self)
{
  mrb_value hash, name;
  mrb_get_args(mrb, "S", &name);
  hash = mrb_mod_cv_get(mrb, GWI_CLASS, mrb_intern_lit(mrb, "events"));
  mrb_hash_delete_key(mrb, hash, name);
  gwi_off(mrb_string_value_ptr(mrb, name));
  return self;
}

static mrb_value
mrb_gwi_get_bg(mrb_state* mrb, mrb_value self)
{
  gwi_Color c;
  gwi_get_background(&c);
  return self;
}

static mrb_value
mrb_gwi_set_bg(mrb_state* mrb, mrb_value self)
{
  gwi_Color c;
  mrb_value obj;
  mrb_get_args(mrb, "o", &obj);
  c.red   = mrb_int(mrb, mrb_funcall(mrb, obj, "red", 0));
  c.green = mrb_int(mrb, mrb_funcall(mrb, obj, "green", 0));
  c.blue  = mrb_int(mrb, mrb_funcall(mrb, obj, "blue", 0));
  c.alpha = mrb_int(mrb, mrb_funcall(mrb, obj, "alpha", 0));
  gwi_set_background(&c);
  return obj;
}

void
mrb_gwi_define_app(mrb_state* mrb, struct RClass *GWI)
{
  GWI_CLASS = GWI;
  mrb_define_class_method(mrb, GWI, "open", mrb_gwi_open, MRB_ARGS_REQ(3));
  mrb_define_class_method(mrb, GWI, "close", mrb_gwi_close, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, GWI, "loop", mrb_gwi_loop, MRB_ARGS_BLOCK());
  mrb_define_class_method(mrb, GWI, "on", mrb_gwi_on, MRB_ARGS_REQ(1)|MRB_ARGS_BLOCK());
  mrb_define_class_method(mrb, GWI, "fire", mrb_gwi_fire, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "off", mrb_gwi_off, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, GWI, "background", mrb_gwi_get_bg, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, GWI, "background=", mrb_gwi_set_bg, MRB_ARGS_REQ(1));
}
