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

static mrb_value
mrb_gwi_open_file(mrb_state* mrb, mrb_value self)
{
  mrb_value result;
  char *str;
  str = gwi_file_open_dialog();
  result = str ? mrb_str_new_cstr(mrb, str) : mrb_nil_value();
  free(str);
  return result;
}

static mrb_value
mrb_gwi_save_file(mrb_state* mrb, mrb_value self)
{
  mrb_value result;
  char *str;
  str = gwi_file_save_dialog();
  result = str ? mrb_str_new_cstr(mrb, str) : mrb_nil_value();
  free(str);
  return result;
}

void
mrb_gwi_define_dialogs(mrb_state* mrb, struct RClass *GWI)
{
mrb_define_class_method(mrb, GWI, "message", mrb_gwi_message, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "error", mrb_gwi_error, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "warning", mrb_gwi_warning, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "info", mrb_gwi_info, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "ask", mrb_gwi_ask, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "accept", mrb_gwi_accept, MRB_ARGS_REQ(2));
mrb_define_class_method(mrb, GWI, "open_file", mrb_gwi_open_file, MRB_ARGS_NONE());
mrb_define_class_method(mrb, GWI, "save_file", mrb_gwi_save_file, MRB_ARGS_NONE());
}
