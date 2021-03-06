#pragma once

/*!
    @file   Dock.h
    @author dasyprocta
 */

#include "Component.h"
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

    virtual void getDockableRect(Rect<sint32>& rect) = 0;
    virtual void setDockableRect(Rect<sint32> const& rect) = 0;

protected:
    Dockable() : m_isDocked(false) {}

private:
    virtual void drawDockable(Graphics& graphics) = 0;

    virtual void onUpdate() {}
    virtual void onDock() {}
    virtual void onUndock() {}

    bool m_isDocked;

    friend Dock;
    friend DockManager;
};

// ============================================================================
class Dock : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Dock);

    enum Type { Left, Top, Right, Bottom, Root };

    Dock::Ptr dock(Dockable::Ptr frame, Dock::Type type);
    void undock(Dockable::Ptr frame);

    Dock::Ptr search(Dockable::Ptr dockable) const;
    bool isDockableExist(Dockable::Ptr dockable) const;
    Dockable::Ptr getDockee() const { return m_dockee; }
    Dock::Ptr getParent() const { return m_parent; }

private:
    void draw(Graphics& graphics);
    void update();
    bool onMouseProcess(MouseCommand const& command);
    bool isPointInner(Point<sint32> const& point);
    Dock::Ptr getDock(Point<sint32> const& point);
    void invokeHandler();

    std::vector<Dock::Ptr>              m_docks;
    Dock::Ptr                           m_parent;
    Dock::Type                          m_type;

    Dockable::Ptr                       m_dockee;
    Rect<sint32>                                m_spliter;

    friend DockManager;
};

// ============================================================================
class DockManager
{
public:
    Dock::Ptr getRoot() const { return m_root; }

    void update(Rect<sint32> const& windowRect, Rect<sint32>& clientRect);
    void draw(Graphics& graphics);

    Dock::Ptr getDock(Point<sint32> const& point) const;
    Dock::Ptr searchDock(Dockable::Ptr dockable) const;
    bool isDockableExist(Dockable::Ptr dockable) const;

    DockManager();

private:
    Dock::Ptr                           m_root;
};

END_NAMESPACE_NYANCO_GUI
