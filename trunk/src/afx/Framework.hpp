#pragma once

/*!
    @file   Framework.hpp
    @author dasyprocta
 */

#include "Application.h"
#include <windows.h>

namespace nyanco
{

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
        nyanco::Application*                appPtr_;

    private:
        Framework();
        Framework(Framework const&);
        void operator = (Framework const&);
    };

} // nyanco namespace

