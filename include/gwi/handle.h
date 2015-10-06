#ifndef GWI_HANDLE_H
#define GWI_HANDLE_H 1

#include "radixtree.h"

struct gwi_panel
{
  gwi_Color  border_color, bg_color, txt_color, shadow_color;
  gwi_Size   border_size, padding;
  int id;
  int cast_shadow;
  struct { int vertical; int horizontal; } shadow_offset;
  int x, y;
  unsigned int width, height, depth;
};

enum gwi_handle_type
{
  GWI_HANDLE_PANEL,
  GWI_HANDLE_CUSTOM,
};

union gwi_handle_value
{
  struct gwi_panel as_panel;
};

struct gwi_Handle
{
  union gwi_handle_value value;
  enum gwi_handle_type type;
  void (*on_draw)(gwi_Handle *);
  rt_tree *events;
};

static gwi_Handle *
gwi_make_handle(void)
{
  gwi_Handle *handle = malloc(sizeof *handle);
  assert(handle);
  handle->events = rt_tree_malloc(255, gwi_free, gwi_alloc, realloc, gwi_free);
  assert(handle->events);
  return handle;
}

#endif
