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
#include <mruby.h>
#include <stdio.h>
#include "gwi.h"

void
mrb_gwi_define_app(mrb_state* mrb, struct RClass *GWI);

void
mrb_gwi_define_dialogs(mrb_state* mrb, struct RClass *GWI);

void
mrb_mruby_gwi_gem_init(mrb_state* mrb) {
  struct RClass *GWI = mrb_define_module(mrb, "GWI");
  mrb_gwi_define_app(mrb, GWI);
  mrb_gwi_define_dialogs(mrb, GWI);
  //mrb_define_class_method(mrb, class_cextension, "c_method", mrb_c_method, MRB_ARGS_NONE());
}

void
mrb_mruby_gwi_gem_final(mrb_state* mrb) {
  gwi_end();
  /* finalizer */
}
