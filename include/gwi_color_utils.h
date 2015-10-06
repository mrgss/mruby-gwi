#include <mruby.h>

#ifndef GWI_COLOR_UTILS_H
#define GWI_COLOR_UTILS_H 1

#ifdef __cplusplus
extern "C" {
#endif

mrb_value
mrb_gwi_color_new(mrb_state *mrb, gwi_Color *color);

void
mrb_gwi_color_from_ruby(mrb_state *mrb, mrb_value color, gwi_Color *result);

void
mrb_gwi_color_to_ruby(mrb_state *mrb, gwi_Color *color, mrb_value result);

#ifdef __cplusplus
}
#endif

#endif
