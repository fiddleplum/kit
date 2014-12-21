#include "Application.h"
#include "Matrix.h"
#include "OpenGL.h"
#include "Transform.h"
#include <ctime>
#include <windows.h>

#ifdef _WIN32

#pragma message("\n----Depends on gdi32.lib.----")

#endif

Matrix44f screenToNDC;
bool loop = true;
Vector2i size;
bool fullscreen = false;
Widget * widget = 0;

#ifdef _WIN32
// Process and window handles.
HINSTANCE hInstance;
HWND hWnd;
HDC deviceContext;
HGLRC openGLContext;

// OpenGL extensions.
int maxIndices = 0;
PFNGLDRAWRANGEELEMENTSWINPROC glDrawRangeElements = 0;
PFNGLGENBUFFERSARBPROC glGenBuffer = 0;
PFNGLBINDBUFFERARBPROC glBindBuffer = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
PFNGLBUFFERDATAARBPROC glBufferData = 0;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture = 0;
#endif

void Application::setTitle (std::string const & text)
{
#ifdef _WIN32
  std::vector<WCHAR> textW;
  textW.resize(MultiByteToWideChar(CP_UTF8, 0, &text[0], text.size(), 0, 0) + 1);
  textW.back() = 0;
  MultiByteToWideChar(CP_UTF8, 0, &text[0], text.size(), &textW[0], textW.size());
  SetWindowTextW(hWnd, &textW[0]);
#endif
}

void Application::quit ()
{
  loop = false;
}

void Application::toggleFullScreen ()
{
  if (fullscreen)
  {
#ifdef _WIN32
  SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  // ShowWindow(hWnd, SW_SHOWNORMAL);
#endif
  }
  else
  {
#ifdef _WIN32
  SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
  SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  // ShowWindow(hWnd, SW_SHOWMAXIMIZED);
#endif
  }
  fullscreen = !fullscreen;

  // Set the size.
#ifdef _WIN32
  RECT rect;
  GetClientRect(hWnd, &rect);
  size.set(rect.right, rect.bottom); // left and top are always 0.
#endif

  screenToNDC = Transform::screenToNDC<float>(size);
  if (widget != 0)
  {
    widget->setBounds(Box2i::minSize(Vector2i::zero(), size));
  }
}

void Application::setWidget (Widget * widget)
{
  ::widget = widget;
}

Vector2i Application::getSize ()
{
  return size;
}

#ifdef _WIN32
LRESULT WINAPI wndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CREATE:
    ::hWnd = hWnd;
    break;
  case WM_CLOSE:
    onClose();
    break;
  case WM_SIZE:
    {
      int newWidth = LOWORD(lParam);
      int newHeight = HIWORD(lParam);
      size.set(newWidth, newHeight);
      screenToNDC = Transform::screenToNDC<float>(size);
      if (widget != 0)
      {
        widget->setBounds(Box2i::minSize(Vector2i::zero(), size));
      }
    }
    break;
  case WM_SYSKEYDOWN:
  case WM_KEYDOWN:
    if (wParam == VK_RETURN && (lParam & 0xffff) == 1 && GetKeyState(VK_MENU) < 0) // alt-enter
    {
      Application::toggleFullScreen();
    }
    break;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

void openWindow ()
{
  // Register the class.
  WNDCLASS wc;
  ZeroMemory(&wc, sizeof(wc));
  wc.lpfnWndProc = wndProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = TEXT("toolbox");
  wc.cbWndExtra = 4;
  RegisterClass(&wc);

  // Create the window. hWnd is set in the WM_CREATE message, called from CreateWindow.
  CreateWindow(TEXT("toolbox"), TEXT("default title"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  // Setup OpenGL with this window.
  deviceContext = GetDC(hWnd);
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 24;
  int pixelFormatIndex = ChoosePixelFormat(deviceContext, &pfd);
  SetPixelFormat(deviceContext, pixelFormatIndex, &pfd);
  openGLContext = wglCreateContext(deviceContext);
  wglMakeCurrent(deviceContext, openGLContext);

  // Initialize OpenGL extensions.
  glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSWINPROC)wglGetProcAddress("glDrawRangeElements");
  glGenBuffer = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
  glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
  glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
  glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
  glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
  if (glDrawRangeElements == 0 || glGenBuffer == 0 || glBindBuffer == 0 || glDeleteBuffers == 0 || glBufferData == 0 || glClientActiveTexture == 0)
  {
    MessageBox(NULL, TEXT("Could not initialize OpenGL Extensions."), 0, 0);
    exit(-1);
  }
  glGetIntegerv(GL_MAX_ELEMENTS_INDICES_WIN, &maxIndices);

  // Unset the OpenGL context.
  wglMakeCurrent(NULL, NULL);
}

void closeWindow ()
{
  if (deviceContext != 0)
  {
    wglMakeCurrent(deviceContext, NULL);
    if (openGLContext != 0)
    {
      wglDeleteContext(openGLContext);
    }
    ReleaseDC(hWnd, deviceContext);
  }
  if (hWnd != 0)
  {
    DestroyWindow(hWnd);
  }
  UnregisterClass(TEXT("toolbox"), hInstance);
}

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // Store the process instance.
  ::hInstance = hInstance;

  // Randomize the random generator.
  srand((unsigned int)time(NULL));

  // Get the command line parameters.
  std::vector<std::string> params;
  int numArgs;
  LPWSTR * args = CommandLineToArgvW(GetCommandLineW(), &numArgs);
  for (int i = 0; i < numArgs; ++i)
  {
    std::string param;
    param.resize(WideCharToMultiByte(CP_UTF8, 0, args[i], -1, 0, 0, NULL, NULL) - 1);
    WideCharToMultiByte(CP_UTF8, 0, args[i], -1, &param[0], param.size(), NULL, NULL);
    params.push_back(param);
  }
  LocalFree(args);
  
  // Open a window.
  openWindow();

  configure(params);

  do
  {
    // Process message
    MSG msg;
    BOOL messageWasPulled = PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
    if (messageWasPulled == TRUE)
    {
      TranslateMessage(&msg); // If we can, post a separate character message onto the message queue for later consumption.
      DispatchMessage(&msg);
    }

    if (widget != 0)
    {
      widget->tick(1000.0f / 60.0f);
      widget->render();
    }
  } while (loop);
  
  // Finalize.
  closeWindow();
}

#endif

