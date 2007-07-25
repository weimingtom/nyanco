/*!
    @file   Framework.cpp
    @author dasyprocta
 */

#include "Framework.hpp"
#include "GraphicsDevice.hpp"

namespace nyanco
{

// ----------------------------------------------------------------------------
void Framework::initialize()
{
    // HACK:
    hwnd_ = createWindow(
        hinstance_,
        L"nyanco framework",
        640,
        480);

    impl::GraphicsDevice& device = impl::GraphicsDevice::GetImplement();
    device.create(hwnd_, 640, 480, false);

    appPtr_->onInitialize();
}

// ----------------------------------------------------------------------------
void Framework::finalize()
{
    appPtr_->onFinalize();
}

// ----------------------------------------------------------------------------
void Framework::run()
{
    MSG     msg             = { 0, 0, 0, 0 };
    bool    isDeviceLost    = false;
    DWORD   prevTime        = ::timeGetTime();

    ::ShowWindow(hwnd_, SW_SHOW);
    ::UpdateWindow(hwnd_);

    GraphicsDevice&     device      = GraphicsDevice::GetInterface();
    LPDIRECT3DDEVICE9   d3dDevice   = device.getD3dDevice();

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            if (isDeviceLost)
            {
                isDeviceLost = false;
            }

            DWORD time = ::timeGetTime();

            appPtr_->onUpdate();

            d3dDevice->Clear(
                0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                D3DCOLOR_XRGB(0, 0, 128), 1.f, 0);

            appPtr_->onDraw();

            if (FAILED(d3dDevice->Present(0, 0, 0, 0)))
            {
                isDeviceLost = true;
            }

            prevTime = time;
        }
    }
}

// ----------------------------------------------------------------------------
void Framework::adjustWindow(
    HWND                                hwnd,
    int                                 clientWidth,
    int                                 clientHeight)
{
    RECT    windowRect;
    RECT    clientRect;

    ::GetWindowRect(hwnd, &windowRect);
    ::GetClientRect(hwnd, &clientRect);

    int const dw        = (windowRect.right - windowRect.left)
                        - (clientRect.right - clientRect.left);
    int const dh        = (windowRect.bottom - windowRect.top)
                        - (clientRect.bottom - clientRect.top);
    int const width     = clientWidth + dw;
    int const height    = clientHeight + dh;

    ::MoveWindow(
        hwnd,
        (::GetSystemMetrics(SM_CXSCREEN) - width) / 2,
        (::GetSystemMetrics(SM_CYSCREEN) - height) / 2,
        width, height, TRUE);
}

// ----------------------------------------------------------------------------
HWND Framework::createWindow(
    HINSTANCE                           hinstance,
    LPCTSTR                             title,
    int                                 width,
    int                                 height,
    bool                                fullscreen)
{
    WNDCLASSEX  wcex;

    wcex.cbSize          = sizeof(WNDCLASSEX);
    wcex.style           = 0;
    wcex.lpfnWndProc     = (WNDPROC)nyanco::Framework::mapProcedure;
    wcex.cbClsExtra      = 0;
    wcex.cbWndExtra      = sizeof(LONG); // Framework class extra field
    wcex.hInstance       = hinstance;
    wcex.hIcon           = ::LoadIcon(0, IDI_APPLICATION);
    wcex.hCursor         = ::LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground   = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName    = 0;
    wcex.lpszClassName   = title;
    wcex.hIconSm         = ::LoadIcon(0, IDI_WINLOGO);

    ::RegisterClassEx(&wcex);
    DWORD style = fullscreen? FullscreenStyle: SizableWindowStyle;

    HWND hwnd = ::CreateWindow(
        title,
        title,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        0,
        0,
        hinstance,
        0);

    adjustWindow(hwnd, width, height);
    ::SetWindowLong(hwnd, 0, reinterpret_cast<LONG>(this));

    return hwnd;
}

// ----------------------------------------------------------------------------
LRESULT Framework::mapProcedure(
    HWND                                hwnd,
    UINT                                msg,
    WPARAM                              wparam,
    LPARAM                              lparam)
{
    Framework* fwp = reinterpret_cast<Framework*>(::GetWindowLong(hwnd, 0));
    if (fwp != 0)
    {
        return fwp->messageProcedure(hwnd, msg, wparam, lparam);
    }
    return ::DefWindowProc(hwnd, msg, wparam, lparam);
}

// ----------------------------------------------------------------------------
LRESULT Framework::messageProcedure(
    HWND                                hwnd,
    UINT                                msg,
    WPARAM                              wparam,
    LPARAM                              lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

} // namespace nyanco

// ----------------------------------------------------------------------------
int WINAPI WinMain(
    HINSTANCE                           hinstance,
    HINSTANCE                           , // hPrevInstance
    LPSTR                               , // szCmdLine
    int                                 ) // iCmdShow
{
    using namespace nyanco;

    extern Application* NyancoCreateApp();

    Framework framework(hinstance, NyancoCreateApp());

    framework.initialize();
    framework.run();
    framework.finalize();

    return 0;
}
