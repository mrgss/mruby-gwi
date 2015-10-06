#ifndef GWI_WIN32_UTF_H
#define GWI_WIN32_UTF_H

#ifdef __cplusplus
extern "C" {
#endif

static wchar_t *
gwi_utf8_to_utf16(const char *str)
{
  int size;
  wchar_t *buff;
  size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
  assert(size);
  buff = gwi_alloc((size + 1) * sizeof *buff);
  assert(MultiByteToWideChar(CP_UTF8, 0, str, -1, buff, size));
  buff[size] = 0;
  return buff;
}

static char *
gwi_utf16_to_utf32(const wchar_t *str)
{
  int   size;
  char *buff;
  size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
  assert(size);
  buff = gwi_alloc((size + 1) * sizeof *buff);
  assert(WideCharToMultiByte(CP_UTF8, 0, str, -1, buff, size, NULL, NULL));
  buff[size] = 0;
  return buff;
}

#ifdef __cplusplus
}
#endif

#endif
