#include <mruby.h>
#include <mruby/variable.h>
#include "gwi.h"

mrb_value
mrb_gwi_color_new(mrb_state *mrb, gwi_Color *color)
{
  mrb_value argv[4];
  struct RClass *GWI, *Color;
  GWI =  mrb_module_get(mrb, "GWI");
  Color = mrb_class_get_under(mrb, GWI, "Color");
  argv[0] = mrb_fixnum_value(color->red);
  argv[1] = mrb_fixnum_value(color->green);
  argv[2] = mrb_fixnum_value(color->blue);
  argv[3] = mrb_fixnum_value(color->alpha);
  return mrb_obj_new(mrb, Color, 4, argv);
}

void
mrb_gwi_color_from_ruby(mrb_state *mrb, mrb_value color, gwi_Color *result)
{
  result->red   = mrb_int(mrb, mrb_funcall(mrb, color, "red", 0));
  result->green = mrb_int(mrb, mrb_funcall(mrb, color, "green", 0));
  result->blue  = mrb_int(mrb, mrb_funcall(mrb, color, "blue", 0));
  result->alpha = mrb_int(mrb, mrb_funcall(mrb, color, "alpha", 0));
}

void
mrb_gwi_color_to_ruby(mrb_state *mrb, gwi_Color *color, mrb_value result)
{
  mrb_funcall(mrb, result, "red=", 1, mrb_fixnum_value(color->red));
  mrb_funcall(mrb, result, "green=", 1, mrb_fixnum_value(color->green));
  mrb_funcall(mrb, result, "blue=", 1, mrb_fixnum_value(color->blue));
  mrb_funcall(mrb, result, "alpha=", 1, mrb_fixnum_value(color->alpha));
}
