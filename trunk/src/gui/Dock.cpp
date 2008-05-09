/*!
    @file   Dock.cpp
    @author dasyprocta
 */

#include "Dock.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

BEGIN_NO_NAMESPACE

sint32 const SplitSize = 5;
sint32 const splitSize = SplitSize;

void calcLeft(Rect& rect, Rect& parent)
{
    rect.setHeight(parent.getHeight());
    rect.setLeft(parent.left);
    rect.setTop(parent.top);
    parent.setLeft(rect.left + rect.getWidth() + splitSize);
    parent.setWidth(parent.getWidth() - rect.getWidth() - splitSize);
}

void calcRight(Rect& rect, Rect& parent)
{
    rect.setHeight(parent.getHeight());
    rect.setLeft(parent.left + parent.getWidth() - rect.getWidth());
    rect.setTop(parent.top);
    parent.setWidth(parent.getWidth() - rect.getWidth() - splitSize);
}

void calcTop(Rect& rect, Rect& parent)
{
    rect.setWidth(parent.getWidth());
    rect.setLeft(parent.left);
    rect.setTop(parent.top);
    parent.setTop(rect.top + rect.getHeight() + splitSize);
    parent.setHeight(parent.getHeight() - rect.getHeight() - splitSize);
}

void calcBottom(Rect& rect, Rect& parent)
{
    rect.setWidth(parent.getWidth());
    rect.setLeft(parent.left);
    rect.setTop(parent.top + parent.getHeight() - rect.getHeight());
    parent.setHeight(parent.getHeight() - rect.getHeight() - splitSize);
}

END_NO_NAMESPACE

// ----------------------------------------------------------------------------
Dock::Ptr Dock::dock(Dockable::Ptr dockee, Dock::Type type)
{
    Dock::Ptr p(new Dock);
    p->m_parent = boost::shared_static_cast<Dock>(shared_from_this());
    p->m_type   = type;
    p->m_dockee = dockee;
    dockee->m_isDocked = true;
    dockee->onDock();
    m_docks.push_back(p);
    return p;
}

// ----------------------------------------------------------------------------
void Dock::undock(Dockable::Ptr dockee)
{
    std::vector<Dock::Ptr>::iterator it = m_docks.begin();
    for (; it != m_docks.end(); ++it)
    {
        if ((*it)->m_dockee == dockee)
        {
            Dock::Ptr deletedDock = *it;
            it = m_docks.erase(it);
            m_docks.insert(it, deletedDock->m_docks.begin(), deletedDock->m_docks.end());
            deletedDock->m_dockee->m_isDocked = false;
            deletedDock->m_dockee->onUndock();
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void Dock::update()
{
    Rect rect, parentRect;
    m_dockee->getDockableRect(rect);
    m_parent->m_dockee->getDockableRect(parentRect);

    // UNDONE: 境界チェック
    typedef void (*Callback)(Rect&, Rect&);
    static Callback calcLocation[] =
    {
        calcLeft, calcTop, calcRight, calcBottom,
    };
    calcLocation[m_type](rect, parentRect);

    m_dockee->setDockableRect(rect);
    m_parent->m_dockee->setDockableRect(parentRect);
    
    foreach (Dock::Ptr dock, m_docks)
    {
        dock->update();
    }
}

// ----------------------------------------------------------------------------
void Dock::draw(Graphics& graphics)
{
    // UNDONE: スプリッタの描画
    Rect rect;
    m_dockee->getDockableRect(rect);
    if (m_type == Dock::Left)
    {
        rect.left = rect.getWidth()+1;
        rect.setWidth(4);
        graphics.setColor(0xff444444);
        graphics.drawFillRect(rect);
        graphics.setColor(0xff888888);
        graphics.drawLine(Point(rect.left, rect.top), Point(rect.left, rect.bottom));
        graphics.setColor(0xff222222);
        graphics.drawLine(Point(rect.right-1, rect.top), Point(rect.right-1, rect.bottom));
    }
    else if (m_type == Dock::Bottom)
    {
        rect.top -= 4;
        rect.setHeight(4);
        graphics.setColor(0xff444444);
        graphics.drawFillRect(rect);
        graphics.setColor(0xff888888);
        graphics.drawLine(Point(rect.left, rect.top), Point(rect.right, rect.top));
        graphics.setColor(0xff222222);
        graphics.drawLine(Point(rect.left, rect.bottom-1), Point(rect.right, rect.bottom-1));
    }

    m_dockee->drawDockable(graphics);
    foreach (Dock::Ptr dock, m_docks)
    {
        dock->draw(graphics);
    }
}

// ----------------------------------------------------------------------------
bool Dock::isPointInner(Point const& point)
{
    Rect rect;
    m_dockee->getDockableRect(rect);
    switch (m_type)
    {
    case Dock::Left:    rect.right  += 4; break;
    case Dock::Top:     rect.bottom += 4; break;
    case Dock::Right:   rect.left   -= 4; break;
    case Dock::Bottom:  rect.top    -= 4; break;
    }
    return rect.isInnerPoint(point.x, point.y);
}

// ----------------------------------------------------------------------------
Dock::Ptr Dock::getDock(Point const& point)
{
    if (isPointInner(point))
        return boost::shared_static_cast<Dock>(shared_from_this());

    foreach (Dock::Ptr dock, m_docks)
    {
        if (Dock::Ptr p = dock->getDock(point))
            return p;
    }
    return Dock::Ptr();
}

// ----------------------------------------------------------------------------
bool Dock::onMouseProcess(MouseCommand const& command)
{
    static bool moving = false;
    if (command.onPushLeft) moving = true;
    if (!command.onDownLeft && !moving) return false;
    if (command.onUpLeft) moving = false;

static sint32 const SplitSize = 5;
    sint32 splitSize = m_parent->m_type == Dock::Root? 0: SplitSize;
    
    Rect rect, parentRect;
    m_dockee->getDockableRect(rect);
    m_parent->m_dockee->getDockableRect(parentRect);

    switch (m_type)
    {
    case Dock::Left:
        rect.right += command.moveX;
        rect.setHeight(parentRect.getHeight());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top);
        parentRect.setLeft(rect.left + rect.getWidth() + splitSize);
        parentRect.setWidth(parentRect.getWidth() - rect.getWidth() - splitSize);
        break;

    case Dock::Right:
        rect.left += command.moveX;
        rect.setHeight(parentRect.getHeight());
        rect.setLeft(parentRect.left + parentRect.getWidth() - rect.getWidth() + splitSize);
        rect.setTop(parentRect.top);
        parentRect.setWidth(parentRect.getWidth() - rect.getWidth() - splitSize);
        break;

    case Dock::Top:
        rect.bottom += command.moveY;
        rect.setWidth(parentRect.getWidth());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top);
        parentRect.setTop(rect.top + rect.getHeight() + splitSize);
        parentRect.setHeight(parentRect.getHeight() - rect.getHeight() - splitSize);
        break;

    case Dock::Bottom:
        rect.top += command.moveY;
        rect.setWidth(parentRect.getWidth());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top + parentRect.getHeight() - rect.getHeight() - splitSize);
        parentRect.setHeight(parentRect.getHeight() - rect.getHeight() - splitSize);
        break;
    }

    m_dockee->setDockableRect(rect);
    m_parent->m_dockee->setDockableRect(parentRect);

    return true;
}

// -----------------------------------------------------------------------------
DockManager::DockManager()
    : m_root(new Dock)
{
    struct RootDockee : public Dockable
    {
        void getDockableRect(Rect& rect) { rect = m_clientRect; }
        void setDockableRect(Rect const& rect) { m_clientRect = rect; }
        void drawDockable(Graphics&) {}
        Rect                            m_clientRect;
    };
    m_root->m_dockee = Dockable::Ptr(new RootDockee);
    m_root->m_type   = Dock::Root;
}

// ----------------------------------------------------------------------------
void DockManager::update(Rect const& windowRect, Rect& clientRect)
{
    m_root->m_dockee->setDockableRect(windowRect);

    foreach (Dock::Ptr dock, m_root->m_docks)
    {
        dock->update();
    }

    m_root->m_dockee->getDockableRect(clientRect);
}

// ----------------------------------------------------------------------------
void DockManager::draw(Graphics& graphics)
{
    foreach (Dock::Ptr dock, m_root->m_docks)
    {
        dock->draw(graphics);
    }
}

// ----------------------------------------------------------------------------
Dock::Ptr DockManager::getDock(Point const& point)
{
    foreach (Dock::Ptr dock, m_root->m_docks)
    {
        if (Dock::Ptr p = dock->getDock(point))
            return p;
    }
    return Dock::Ptr();
}

END_NAMESPACE_NYANCO_GUI
