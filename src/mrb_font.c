#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/data.h>
#include "gwi.h"

static void
mrb_font_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
    gwi_free_font(ptr);
}

static void
mrb_font_opts_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
    gwi_free(ptr);
}

const struct mrb_data_type mrbal_gwi_font_data_type = {
  "gwi/Font",
  mrb_font_free
};

const struct mrb_data_type mrbal_gwi_font_options_data_type = {
  "gwi/FontOptions",
  mrb_font_opts_free
};

mrb_value
mrb_gwi_font_new(mrb_state* mrb, gwi_Font *fnt)
{
  mrb_value argv[2];
  struct RClass *GWI, *Font;
  mrb_value rb_font;
  GWI =  mrb_module_get(mrb, "GWI");
  Font = mrb_class_get_under(mrb, GWI, "Font");
  argv[0] = mrb_str_new_cstr(mrb, "");
  argv[1] = mrb_fixnum_value(16);
  rb_font = mrb_obj_new(mrb, Font, 2, argv);
  if (DATA_PTR(rb_font)) {
    gwi_free_font(DATA_PTR(rb_font));
  }
  DATA_PTR(rb_font) = fnt;
  return rb_font;
}

mrb_value
mrb_gwi_font_init(mrb_state* mrb, mrb_value self)
{
  mrb_int count;
  char *name;
  mrb_int size;
  gwi_FontOptions *opts;
  count = mrb_get_args(mrb, "zi|d", &name, &size, &opts, &mrbal_gwi_font_options_data_type);
  if (count < 3) opts = NULL;
  DATA_TYPE(self) = &mrbal_gwi_font_data_type;
  DATA_PTR(self)  = gwi_load_font(name, size, opts);
  return self;
}

mrb_value
mrb_gwi_font_is_valid(mrb_state* mrb, mrb_value self)
{
  return mrb_bool_value(gwi_font_is_valid(DATA_PTR(self)));
}

mrb_value
mrb_gwi_font_opt_init(mrb_state* mrb, mrb_value self)
{
  DATA_TYPE(self) = &mrbal_gwi_font_options_data_type;
  DATA_PTR(self)  = gwi_alloc(sizeof(gwi_FontOptions));
  return self;
}

void
mrb_gwi_define_font(mrb_state* mrb, struct RClass *GWI)
{
  struct RClass *Font, *FontOptions;
  Font = mrb_define_class_under(mrb, GWI, "Font", mrb->object_class);
  FontOptions = mrb_define_class_under(mrb, Font, "Options", mrb->object_class);

  mrb_define_method(mrb, Font, "initialize", mrb_gwi_font_init, MRB_ARGS_REQ(2)|MRB_ARGS_OPT(1));
  mrb_define_method(mrb, Font, "valid?", mrb_gwi_font_is_valid, MRB_ARGS_NONE());


  mrb_define_method(mrb, FontOptions, "initialize", mrb_gwi_font_opt_init, MRB_ARGS_NONE());
}
