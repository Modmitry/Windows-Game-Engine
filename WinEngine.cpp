#define WIN32_LEAN_AND_MEAN
#include "WinEngine.h"
#include <windows.h>
#include <stdlib.h>

uint32_t buff[HEIGHT_SCREEN][WIDTH_SCREEN] = { 0 };

static HINSTANCE hinst = 0;
static DWORD ticks = 0;
static bool is_active = true;
static POINT cursor_pos;
static bool quited = false;
static LARGE_INTEGER qpc_frequency = { 0 };
static LARGE_INTEGER qpc_ref_time = { 0 };
//------------------------------------------------------
bool is_window_active()
{
  return is_active;
}
//------------------------------------------------------
void clear_buffer()
{
  memset(buff, 0, sizeof(buff));
}
//------------------------------------------------------
bool is_key_pressed(int button_vk_code)
{
  return is_active && (GetAsyncKeyState(button_vk_code) & 0x8000);
}
//------------------------------------------------------
bool is_mouse_button_pressed(int mouse_button_index)
{
  if (!is_active)
    return false;

  if (mouse_button_index == 0)
    return GetAsyncKeyState(VK_LBUTTON) != 0;

  if (mouse_button_index == 1)
    return GetAsyncKeyState(VK_RBUTTON) != 0;

  return false;
}
//------------------------------------------------------
int get_cursor_x()
{
  return cursor_pos.x;
}
//------------------------------------------------------
int get_cursor_y()
{
  return cursor_pos.y;
}
//------------------------------------------------------
void Quit()
{
  quited = true;
}
//------------------------------------------------------
static void CALLBACK update_proc(HWND hwnd)
{
  if (quited)
    return;

  is_active = GetActiveWindow() == hwnd;

  GetCursorPos(&cursor_pos);
  ScreenToClient(hwnd, &cursor_pos);

  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);

  float dt = float((unsigned long)(t.QuadPart - qpc_ref_time.QuadPart)) / qpc_frequency.QuadPart;
  if (dt > 0.1f)
    dt = 0.1f;

  Act(dt);

  if (!quited)
  {
    Draw();
    RedrawWindow(hwnd, NULL, 0, RDW_INVALIDATE | RDW_UPDATENOW);
  }

  qpc_ref_time = t;
}
//------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      BITMAPINFOHEADER bih;
      bih.biSize = sizeof(bih);
      bih.biWidth = WIDTH_SCREEN;
      bih.biHeight = -HEIGHT_SCREEN;
      bih.biPlanes = 1;
      bih.biBitCount = 32;
      bih.biCompression = BI_RGB;
      bih.biSizeImage = 0;
      bih.biXPelsPerMeter = 96;
      bih.biYPelsPerMeter = 96;
      bih.biClrUsed = 0;
      bih.biClrImportant = 0;
      SetDIBitsToDevice(
        hdc,
        0, 0,
          WIDTH_SCREEN, HEIGHT_SCREEN,
        0, 0,
        0, HEIGHT_SCREEN,
          buff,
        (BITMAPINFO*)&bih,
        DIB_RGB_COLORS);

      EndPaint(hwnd, &ps);
    }
  break;
  case WM_QUIT:
  case WM_DESTROY:
    quited = true;
    break;
  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}
//------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  SetProcessDPIAware();
  hinst = hInstance;
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  WNDCLASSEXA wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = 0;
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wcex.lpszMenuName = 0;
  wcex.lpszClassName = "GameTemplateWndClass";
  wcex.hIconSm = 0;
  RegisterClassExA(&wcex);

  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = WIDTH_SCREEN;
  rect.bottom = HEIGHT_SCREEN;
  AdjustWindowRectEx(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0);

  HWND hwnd = CreateWindowA(wcex.lpszClassName, "Game", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
    CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

  if (!hwnd)
    return 0;

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  QueryPerformanceFrequency(&qpc_frequency);
  QueryPerformanceCounter(&qpc_ref_time);

  ticks = GetTickCount64();
  Init();

  MSG message;
  while (!quited)
  {
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
    update_proc(hwnd);
    Sleep(0);
  }

  Finish();

  return (int)message.wParam;
}
//------------------------------------------------------