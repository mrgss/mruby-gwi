#ifndef GWI_DRAWINGS_H
#define GWI_DRAWINGS_H

#include "gwi/handle.h"

#ifdef __cplusplus
extern "C" {
#endif

static int
total_panel_width(struct gwi_panel *panel)
{
  return panel->width + panel->border.left + panel->border.right;
}

static int
total_panel_width(struct gwi_panel *panel)
{
  return panel->height + panel->border.top + panel->border.bottom;
}

void
gwi_Panel_draw(gwi_Handle *handle)
{
  struct gwi_panel *panel;
  gwi_Color bg;
  int w, h;
  HDC base_hdc, hdc1, hdc2;
  RECT rect;
  HBITMAP bmp1, bmp2, bmp3;
  HGDIOBJ old_bmp1, old_bmp2, old_bmp_3;
  HBRUSH brush;
  COLORREF shadow, transparent;
  gwi_get_background(&bg);
  panel    = (gwi_Panel *)handle;
  w        = total_panel_width(panel);
  h        = total_panel_height(panel);
  base_hdc = GetDC(hwnd);
  transparent = RGB(bg->red, bg->green, bg->blue);
  shadow      = RGB(panel->shadow_color.red, panel->shadow_color.green, panel->shadow_color.blue);

  rect.left   = panel->x;
  rect.top    = panel->y;
  rect.right  = panel->x + w;
  rect.bottom = panel->y + h;

  hdc1     = CreateCompatibleDC(base_hdc);
  bmp1     = CreateCompatibleBitmap(hdc, w, h);
  old_bmp1 = SelectObject(hdc, bmp1);

  hdc2     = CreateCompatibleDC(base_hdc);
  bmp2     = CreateCompatibleBitmap(hdc, w, h);
  old_bmp2 = SelectObject(hdc, bmp2);

  hdc3     = CreateCompatibleDC(base_hdc);
  bmp3     = CreateCompatibleBitmap(hdc, w, h);
  old_bmp3 = SelectObject(hdc, bmp3);

  ReleaseDC(hwnd, base_hdc);

  brush = CreateSolidBrush(shadow);
  FillRect(hdc1, &rect, brush);
  FillRect(hdc2, &rect, brush);
  BitBlt(hdc3, 0, 0, w, h, base_hdc, 0, 0, SRCCOPY);
  DeleteObject(brush);

  CreateDrawing(hdc2);

  TransparentBlt(hdc1, panel->shadow_offset.horizontal, panel->shadow_offset.vertical, w, h,
        hdc2, 0, 0, w, h, shadow);
}

#ifdef __cplusplus
}
#endif

#endif
