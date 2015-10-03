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
  handle.GWI = GWI_CLASS;
  mrb_value hash, name, block;
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
mrb_gwi_message(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  gwi_message(title, msg);
  return mrb_nil_value();
}

static mrb_value
mrb_gwi_error(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  gwi_error(title, msg);
  return mrb_nil_value();
}


static mrb_value
mrb_gwi_warning(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  gwi_warning(title, msg); 
  return mrb_nil_value();
}

static mrb_value
mrb_gwi_info(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  gwi_info(title, msg);
  return mrb_nil_value();
}

static mrb_value
mrb_gwi_ask(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  return mrb_bool_value(gwi_ask(title, msg));
}

static mrb_value
mrb_gwi_accept(mrb_state* mrb, mrb_value self)
{
  char *msg, *title;
  mrb_get_args(mrb, "zz", &title, &msg);
  return mrb_bool_value(gwi_accept(title, msg));
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
  mrb_define_class_method(mrb, GWI, "message", mrb_gwi_message, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "error", mrb_gwi_error, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "warning", mrb_gwi_warning, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "info", mrb_gwi_info, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "ask", mrb_gwi_ask, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, GWI, "accept", mrb_gwi_accept, MRB_ARGS_REQ(2));
}