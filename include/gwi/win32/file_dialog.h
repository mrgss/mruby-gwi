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
#ifndef GWI_WIN32_FILE_DIALOG_H
#define GWI_WIN32_FILE_DIALOG_H 1

 #ifdef __cplusplus
  extern "C" {
 #endif

#define GWI_FILE_DIALOG_BUFFER_SIZE 512

static void
win32_gwi_file_options(TCHAR *szFileName, OPENFILENAME *ofn)
{
  ZeroMemory(ofn, sizeof(*ofn));
  ofn->lStructSize = sizeof(*ofn);
  ofn->hwndOwner = hwnd;
  ofn->lpstrFilter = NULL;
  ofn->lpstrFile = szFileName;
  ofn->nMaxFile = MAX_PATH;
  ofn->lpstrDefExt = NULL;
}

static char *
win32_gwi_file_make_character_result(TCHAR *szFileName)
{
  size_t size;
  char *result, *cp;
#ifdef _UNICODE
    cp = gwi_utf16_to_utf32(szFileName);
    size = strlen(cp);
    result = malloc(size + 1);
    assert(result);
    strncpy(result, cp, size);
    result[size] = 0;
    free(cp);
#else
    size = strlen(szFileName);
    result = malloc(size + 1);
    assert(result);

    strncpy(result, szFileName, size);
    result[size] = 0;
#endif
  return result;
}

char *
gwi_file_open_dialog(void)
{
  OPENFILENAME ofn;
  TCHAR szFileName[GWI_FILE_DIALOG_BUFFER_SIZE] = TEXT("");
  win32_gwi_file_options(szFileName, &ofn);
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if(GetOpenFileName(&ofn))
  {
    szFileName[GWI_FILE_DIALOG_BUFFER_SIZE - 1] = 0;
    return win32_gwi_file_make_character_result(szFileName);
  }
  return NULL;
}

char *
gwi_file_save_dialog(void)
{
  OPENFILENAME ofn;
  TCHAR szFileName[GWI_FILE_DIALOG_BUFFER_SIZE] = TEXT("");
  win32_gwi_file_options(szFileName, &ofn);
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

  if(GetSaveFileName(&ofn))
  {
    szFileName[GWI_FILE_DIALOG_BUFFER_SIZE - 1] = 0;
    return win32_gwi_file_make_character_result(szFileName);
  }
  return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
