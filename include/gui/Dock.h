#pragma once

/*!
    @file   Dock.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <vector>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class Dockable;
class Dock;
class DockManager;
class Graphics;

// ============================================================================
class Dockable
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Dockable);
    bool isDocked() const { return m_isDocked; }

protected:
    Dockable() : m_isDocked(false) {}

private:
    virtual void getDockableRect(Rect& rect) = 0;
    virtual void setDockableRect(Rect const& rect) = 0;
    virtual void drawDockable(Graphics& graphics) = 0;

    virtual void onDock() {}
    virtual void onUndock() {}

    bool m_isDocked;

    friend Dock;
    friend DockManager;
};

// ============================================================================
class Dock : public boost::enable_shared_from_this<Dock>
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Dock);

    enum Type { Root, Left, Top, Right, Bottom };

    Dock::Ptr dock(Dockable::Ptr frame, Dock::Type type);
    void undock(Dockable::Ptr frame);

private:
    void draw(Graphics& graphics);
    void update();

    std::vector<Dock::Ptr>              m_docks;
    Dock::Ptr                           m_parent;
    Dock::Type                          m_type;

    Dockable::Ptr                       m_dockee;

    friend DockManager;
};

// ============================================================================
class DockManager
{
public:
    Dock::Ptr getRoot() const { return m_root; }

    void update(Rect const& windowRect, Rect& clientRect);
    void draw(Graphics& graphics);
    void onMouseProcess(MouseCommand const& command);

    DockManager();

private:
    Dock::Ptr                           m_root;
};

END_NAMESPACE_NYANCO_GUI
