#include <mruby.h>
#include "gwi.h"

void 
mrb_gwi_define_app(mrb_state* mrb, struct RClass *GWI);

void
mrb_mruby_gwi_gem_init(mrb_state* mrb) {
  struct RClass *GWI = mrb_define_module(mrb, "GWI");
  mrb_gwi_define_app(mrb, GWI);
  //mrb_define_class_method(mrb, class_cextension, "c_method", mrb_c_method, MRB_ARGS_NONE());
}

void
mrb_mruby_gwi_gem_final(mrb_state* mrb) {
  gwi_end();
  /* finalizer */
}
