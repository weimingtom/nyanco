#pragma once

/*!
    @file   WindowManager.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Frame.h"
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
        Point const&                point,
        std::string const&          text,
        Color                       color) = 0;

    virtual void attach(
        Frame<>::Ptr                framePtr) = 0;

    virtual void detach(
        Frame<>::Ptr                framePtr) = 0;

    virtual void detach(
        ComponentId                 id) = 0;

    virtual Frame<>::Ptr search(
        ComponentId                 id) = 0;

    virtual void activate(
        Frame<>::Ptr                framePtr) = 0;

    virtual Frame<>::Ptr getActiveWindow() const = 0;

    virtual ContextMenuPtr getContextMenu() const = 0;

    virtual Rect const& getClientRect() const = 0;

    virtual Dock::Ptr dock(
        Dockable::Ptr                   dockable,
        Dock::Type                      type) = 0;

    virtual void undock(
        Dockable::Ptr                   dockable) = 0;

    static WindowManager& GetInterface();
};

END_NAMESPACE_NYANCO_GUI
