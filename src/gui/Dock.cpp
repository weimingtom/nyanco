/*!
    @file   Dock.cpp
    @author dasyprocta
 */

#include "Dock.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Dock::Ptr Dock::dock(Dockable::Ptr dockee, Dock::Type type)
{
    Dock::Ptr p(new Dock);
    p->m_parent = shared_from_this();
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
    static sint32 const SplitSize = 5;
    sint32 splitSize = m_parent->m_type == Dock::Root? 0: SplitSize;

    Rect rect, parentRect;
    m_dockee->getDockableRect(rect);
    m_parent->m_dockee->getDockableRect(parentRect);

    // UNDONE: 境界チェック
    switch (m_type)
    {
    case Dock::Left:
        rect.setHeight(parentRect.getHeight());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top);
        parentRect.setLeft(rect.left + rect.getWidth() + splitSize);
        parentRect.setWidth(parentRect.getWidth() - rect.getWidth() - splitSize);
        break;

    case Dock::Right:
        rect.setHeight(parentRect.getHeight());
        rect.setLeft(parentRect.left + parentRect.getWidth() - rect.getWidth() + splitSize);
        rect.setTop(parentRect.top);
        parentRect.setWidth(parentRect.getWidth() - rect.getWidth() - splitSize);
        break;

    case Dock::Top:
        rect.setWidth(parentRect.getWidth());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top);
        parentRect.setTop(rect.top + rect.getHeight() + splitSize);
        parentRect.setHeight(parentRect.getHeight() - rect.getHeight() - splitSize);
        break;

    case Dock::Bottom:
        rect.setWidth(parentRect.getWidth());
        rect.setLeft(parentRect.left);
        rect.setTop(parentRect.top + parentRect.getHeight() - rect.getHeight() - splitSize);
        parentRect.setHeight(parentRect.getHeight() - rect.getHeight() - splitSize);
        break;
    }

    m_dockee->setDockableRect(rect);
    m_parent->m_dockee->setDockableRect(rect);

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
        rect.left = rect.getWidth();
        rect.setWidth(5);
        graphics.setColor(0xff444444);
        graphics.drawFillRect(rect);
        graphics.setColor(0xff888888);
        graphics.drawLine(Point(rect.left+1, rect.top), Point(rect.left+1, rect.bottom));
        graphics.setColor(0xff222222);
        graphics.drawLine(Point(rect.right-1, rect.top), Point(rect.right-1, rect.bottom));
    }

    m_dockee->drawDockable(graphics);
    foreach (Dock::Ptr dock, m_docks)
    {
        dock->draw(graphics);
    }
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
void DockManager::onMouseProcess(MouseCommand const& command)
{
    
}

END_NAMESPACE_NYANCO_GUI
