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
#ifndef GWI_WIN32_WINDOW_H
#define GWI_WIN32_WINDOW_H 1

#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "gwi/win32/utf.h"

#ifdef __cplusplus
extern "C" {
#endif

static const TCHAR GWI_CLASS_NAME[] = TEXT("GWI_WINDOW_CLASS");

static int gwi_updating = 0;

static void
gwi_win32_gdi_repaint(HWND hwnd)
{
      PAINTSTRUCT ps;
      RECT rc;
      HBRUSH brush;
      gwi_Color bg;
      BeginPaint(hwnd, &ps);
      GetClientRect(
          hwnd,
          &rc
      );
      gwi_get_background(&bg);
      brush = CreateSolidBrush(RGB(bg.red, bg.green, bg.blue));

      // Draw a rectangle.
      FillRect(ps.hdc, &rc, brush);

      DeleteObject(brush);

      EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK
gwi_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
      /*
        case WM_MOVE:
          {
            short int x, y;
            x = (int)(short) LOWORD(lParam);
            y = (int)(short) HIWORD(lParam);
          }
          break;
        case WM_SIZE:
          switch (wParam)
          {
            case SIZE_MAXHIDE:
              break;
            case SIZE_MAXIMIZED:
              break;
            case SIZE_MINIMIZED:
              break;
            case SIZE_RESTORED:
              break;
            default:
              break;
          }
          */



        case WM_PAINT:
          gwi_win32_gdi_repaint(hwnd);
          return 0;
        case WM_SIZE:
          gwi_win32_gdi_repaint(hwnd);
          return 0;
        case WM_CLOSE:
            gwi_updating = 0;
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            gwi_updating = 0;
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 1;
}

static WNDCLASSEX gwi_window_class;
static HWND gwi_window_hwnd = NULL;
static MSG gwi_window_msg;

#define wc   gwi_window_class
#define hwnd gwi_window_hwnd
#define msg  gwi_window_msg

static void
gwi_register_window_class(void)
{
  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = gwi_window_proc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = GetModuleHandle(NULL);
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = GWI_CLASS_NAME;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  assert(RegisterClassEx(&wc));
}

static void
gwi_create_window_handle(TCHAR *title, size_t width, size_t height)
{
  hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        GWI_CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, GetModuleHandle(NULL), NULL);
  assert(hwnd != NULL);
  ShowWindow(hwnd, 1);
  UpdateWindow(hwnd);
}



static void
gwi_open_window(const char *title, size_t width, size_t height)
{
  gwi_register_window_class();
#ifdef _UNICODE
  {
     TCHAR *title_tchar = gwi_utf8_to_utf16(title);
     gwi_create_window_handle(title_tchar, width, height);
     free(title_tchar);
  }
#else
  gwi_create_window_handle(title, width, height);
#endif
}

static void
gwi_close_window(void)
{
  UnregisterClass(GWI_CLASS_NAME, GetModuleHandle(NULL));
  hwnd = NULL;
}

static int
gwi_update_window(void *context, gwi_loop_callback callback)
{
  BOOL bRet;
    gwi_updating = 1;
    while( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
      if (bRet != -1) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (callback) callback(context);
      }
    }
    return gwi_updating;
}

void
gwi_message(const char *title, const char *msg)
{
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  MessageBox(hwnd, wtitle, wmsg, MB_OK|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  MessageBox(hwnd, title, msg, MB_OK|MB_TASKMODAL);
#endif
}

void
gwi_error(const char *title, const char *msg)
{
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  MessageBox(hwnd, wtitle, wmsg, MB_OK|MB_ICONERROR|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  MessageBox(hwnd, title, msg, MB_OK|MB_ICONERROR|MB_TASKMODAL);
#endif
}

void
gwi_info(const char *title, const char *msg)
{
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  MessageBox(hwnd, wtitle, wmsg, MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  MessageBox(hwnd, title, msg, MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
#endif
}

void
gwi_warning(const char *title, const char *msg)
{
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  MessageBox(hwnd, wtitle, wmsg, MB_OK|MB_ICONWARNING|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  MessageBox(hwnd, title, msg, MB_OK|MB_ICONWARNING|MB_TASKMODAL);
#endif
}

int
gwi_ask(const char *title, const char *msg)
{
  int result;
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return 0;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  result = MessageBox(hwnd, wtitle, wmsg, MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  result = MessageBox(hwnd, title, msg, MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
#endif
  return result == IDYES;
}

int
gwi_accept(const char *title, const char *msg)
{
  int result;
#ifdef _UNICODE
  wchar_t *wmsg, *wtitle;
#endif
  if (!gwi_updating) return 0;
#ifdef _UNICODE
  wmsg = gwi_utf8_to_utf16(msg);
  wtitle = gwi_utf8_to_utf16(title);
  result = MessageBox(hwnd, wtitle, wmsg, MB_OKCANCEL|MB_ICONQUESTION|MB_TASKMODAL);
  free(wmsg);
  free(wtitle);
#else
  result = MessageBox(hwnd, title, msg, MB_OKCANCEL|MB_ICONQUESTION|MB_TASKMODAL);
#endif
  return result == IDOK;
}

void
gwi_refresh(void)
{
  RECT rect;
  GetClientRect(hwnd, &rect);
  InvalidateRect(hwnd, &rect, FALSE);
}

#ifdef __cplusplus
}
#endif

#include "gwi/win32/file_dialog.h"
#include "gwi/win32/font.h"
#include "gwi/win32/drawings.h"

#endif
