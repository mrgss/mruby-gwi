#include <windows.h>
#include <assert.h>
#include <stdlib.h>

static const TCHAR GWI_CLASS_NAME[] = TEXT("GWI_WINDOW_CLASS");

static int gwi_updating = 0;

LRESULT CALLBACK 
gwi_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
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
    return 0;
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

static wchar_t *
gwi_utf8_to_utf16(const char *str)
{
  int size;
  wchar_t *buff;
  size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
  assert(size);
  buff = malloc((size + 1) * sizeof *buff);
  assert(buff);
  assert(MultiByteToWideChar(CP_UTF8, 0, str, -1, buff, size));
  buff[size] = 0;
  return buff;
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
  gwi_updating = 1;
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
  
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (callback) callback(context);
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