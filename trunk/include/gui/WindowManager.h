#pragma once

/*!
    @file   WindowManager.h
    @author dasyprocta
 */

#include "Window.h"
#include "ContextMenu.h"
#include "Dock.h"
#include <d3d9.h>
#include <string>

BEGIN_NAMESPACE_NYANCO_GUI

class Graphics;

class WindowManager
{
public:
    virtual void drawText(
        Point<sint32> const&                point,
        std::string const&          text,
        Color                       color) = 0;

    virtual void attach(
        Window::Ptr                framePtr) = 0;

    virtual void detach(
        Window::Ptr                framePtr) = 0;

    virtual void detach(
        ComponentId                 id) = 0;

    virtual Window::Ptr search(
        ComponentId                 id) = 0;

    virtual void activate(
        Window::Ptr                framePtr) = 0;

    virtual Window::Ptr getActiveWindow() const = 0;

    virtual ContextMenu::Ptr getContextMenu() const = 0;
    virtual void setContextMenu(ContextMenu::Ptr menu) = 0;

    virtual Rect<sint32> const& getClientRect() const = 0;

    virtual Dock::Ptr getRootDock() const = 0;

    virtual Dock::Ptr dock(
        DockableWindow::Ptr             wnd,
        Dock::Type                      type) = 0;

    virtual Dock::Ptr dock(
        DockableWindow::Ptr             wnd,
        Dock::Type                      type,
        Dock::Ptr                       dock) = 0;

    virtual void undock(
        DockableWindow::Ptr             wnd) = 0;

    static WindowManager& GetInterface();
};

END_NAMESPACE_NYANCO_GUI
