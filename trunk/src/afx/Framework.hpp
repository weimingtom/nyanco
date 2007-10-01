#pragma once

/*!
    @file   Framework.hpp
    @author dasyprocta
 */

#include "nyanco.hpp"
#include <windows.h>
#include <d3dx9.h>

namespace nyanco
{
    class Application;
    class GuiInterface;

    class Framework
    {
    public:
        void initialize();
        void finalize();
        void run();

        Framework(
            HINSTANCE                       hinstance,
            nyanco::Application*            appPtr)
            : hinstance_(hinstance), appPtr_(appPtr) {}

    private:
        LRESULT messageProcedure(HWND, UINT, WPARAM, LPARAM);
        static LRESULT CALLBACK mapProcedure(HWND, UINT, WPARAM, LPARAM);

        HWND createWindow(
            HINSTANCE                       hinstance,
            LPCTSTR                         title,
            int                             width,
            int                             height,
            bool                            fullscreen = false);

        void adjustWindow(
            HWND                            hwnd,
            int                             clientWidth,
            int                             clientHeight);

        void reset();

        enum Style
        {
            SizableWindowStyle
                = WS_OVERLAPPEDWINDOW,
            FixedWindowStyle
                = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            FullscreenStyle
                = WS_VISIBLE | WS_POPUP,
        };

    private:
        HINSTANCE                           hinstance_;
        HWND                                hwnd_;

        Application*                        appPtr_;
        GuiInterface*                       guiPtr_;

    private:
        Framework();
        Framework(Framework const&);
        void operator = (Framework const&);
    };

} // nyanco namespace

