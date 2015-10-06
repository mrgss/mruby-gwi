#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "gwi.h"
#include "gwi/win32/utf.h"

#ifndef GWI_WIN32_FONT_H
#define GWI_WIN32_FONT_H 1

#ifdef __cplusplus
extern "C" {
#endif

struct gwi_Font
{
  gwi_Color color;
  gwi_Color out_color;
  gwi_Color shadow_color;
  HFONT handle;
};

static void
gwi_font_init(gwi_Font *font)
{
  font->color.red = font->color.green = font->color.blue = font->color.alpha = 255;
  font->out_color.red = font->out_color.green = font->out_color.blue = font->out_color.alpha =
  font->shadow_color.red = font->shadow_color.green = font->shadow_color.blue = font->shadow_color.alpha = 0;
}

gwi_Font *
gwi_load_font(const char *name, size_t size, gwi_FontOptions *options)
{
  char u, s, i, b;
  gwi_Font *fnt;
#ifdef _UNICODE
  TCHAR *str;
  str = gwi_utf8_to_utf16(name);
#endif
  fnt = gwi_alloc(sizeof *fnt);
  if (options) {
    u = options->underline;
    i = options->italic;
    s = options->strike;
    b = options->bold;
  } else {
    u = i = s = b = FALSE;
  }
  fnt->handle = CreateFont(size, 0, 0, 0, b ? FW_BOLD : FW_NORMAL,
    i, u, s,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, FF_DONTCARE,
#ifdef _UNICODE
    str
#else
    name
#endif
  );
  if (!fnt->handle) {
#ifdef _UNICODE
      gwi_free(str);
#endif
    gwi_free(fnt);
    return NULL;
  }
  gwi_font_init(fnt);
#ifdef _UNICODE
  gwi_free(str);
#endif
  return fnt;
}

void
gwi_free_font(gwi_Font *font)
{
  DeleteObject(font->handle);
  gwi_free(font);
}

gwi_Font *
gwi_select_font(void)
{
  CHOOSEFONT cf;
  LOGFONT  fl;
  cf.lStructSize = sizeof(cf);
  cf.hwndOwner   = hwnd;
  cf.lpLogFont   = &fl;
  cf.Flags       = CF_EFFECTS|CF_FORCEFONTEXIST;
  cf.rgbColors   = RGB(0,0,0);
  cf.lpTemplateName = NULL;
  cf.nFontType      = REGULAR_FONTTYPE;
  if (ChooseFont(&cf)) {
    gwi_Font *fnt;
    fnt = gwi_alloc(sizeof *fnt);
    fnt->handle = CreateFontIndirect(&fl);
    assert(fnt->handle);
    gwi_font_init(fnt);
    return fnt;
  }
  return NULL;
}

int
gwi_font_is_valid(gwi_Font *fnt)
{
  return fnt && fnt->handle;
}

#ifdef __cplusplus
}
#endif

#endif
