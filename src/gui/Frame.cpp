/*!
    @file   Frame.cpp
    @author dasyprocta
 */

#include "Frame.h"
#include "TitleBar.h"
#include "Graphics.h"
#include "WindowManager.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

using boost::bind;
using boost::ref;

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
void Frame<>::create(
    Frame<>::Ptr                        frame,
    ComponentId                         id,
    std::string const&                  caption,
    uint32                              width,
    uint32                              height)
{
    frame->setId(id);

    m_titleBar = TitleBar::Create(-1, caption, frame);
    m_titleBar->setCaption(caption);

    {
        Rect<sint32>& location = frame->getLocation();
        location.left = location.top = 0;
        location.right          = width;
        location.bottom         = height;
    }

    {
        frame->margin_.left = frame->margin_.right = 3;
        frame->margin_.top  = frame->margin_.bottom = 3;
    }
    relocateChildren();
//    frame->onInitialize();
}

// ----------------------------------------------------------------------------
void Frame<>::moveTo(int x, int y)
{
    Rect<sint32>& location = getLocation();
    location.right     = x + location.getWidth();
    location.bottom    = y + location.getHeight();
    location.left      = x;
    location.top       = y;
}

// ----------------------------------------------------------------------------
void Frame<>::move(int x, int y)
{
    Rect<sint32>& location = getLocation();
    location.left  += x;
    location.right += x;
    location.top   += y;
    location.bottom += y;

    m_titleBar->move(x, y);
    static_cast<Component::Ptr>(m_panel)->move(x, y);
}

// ----------------------------------------------------------------------------
void Frame<>::attach(Component::Ptr component)
{
    component->setEventServer(this);
    m_panel->attach(component);
}

// ----------------------------------------------------------------------------
void Frame<>::detach(Component::Ptr component)
{
    component->setEventServer(0);
    m_panel->detach(component);
}

// ----------------------------------------------------------------------------
void Frame<>::draw(
    Graphics&                       graphics)
{
    ComponentGraphics g(graphics);

    Rect<sint32> const& location = getLocation();

    g.drawFrame(location);

    // title bar
    static_cast<Component::Ptr>(m_titleBar)->draw(graphics);
    
    // 子の描画
    static_cast<Component::Ptr>(m_panel)->draw(graphics);
    //std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
void Frame<>::update()
{
    static_cast<Component::Ptr>(m_panel)->update();
    //std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::update, _1));
}

// ----------------------------------------------------------------------------
void Frame<>::relocateChildren()
{
    Rect<sint32>& location = getLocation();

    // title bar
    int locationY = location.top + margin_.top;
    m_titleBar->relocate(location.left + margin_.left, location.getWidth() - margin_.left * 2, locationY);
    // component
    locationY += static_cast<Component::Ptr>(m_titleBar)->getHeight() + margin_.top;
    static_cast<Component::Ptr>(m_panel)->relocate(location.left + margin_.left, location.getWidth() - margin_.left * 2, locationY);

    location.bottom = locationY + static_cast<Component::Ptr>(m_panel)->getHeight() + margin_.bottom;;
}

// ----------------------------------------------------------------------------
ComponentPtr Frame<>::getHitComponent(int x, int y)
{
    if (isPointInner(Point<sint32>(x, y)))
    {
        // title bar
        if (m_titleBar->isPointInner(Point<sint32>(x, y)))
            return m_titleBar;

        // 子コンポーネントにヒット
        Component::Ptr hit = static_cast<Component::Ptr>(m_panel)->checkHit(x, y);
        if (hit != 0) return hit;
    }
    return ComponentPtr();
}

// ----------------------------------------------------------------------------
void Frame<>::getDockableRect(Rect<sint32>& rect)
{
    rect = getLocation();
}

// ----------------------------------------------------------------------------
void Frame<>::setDockableRect(Rect<sint32> const& rect)
{
    getLocation() = rect;
    // title bar
    int locationY = rect.top + margin_.top;
    m_titleBar->relocate(rect.left + margin_.left, rect.getWidth() - margin_.left * 2, locationY);
    // component
    locationY += static_cast<Component::Ptr>(m_titleBar)->getHeight() + margin_.top;
    static_cast<Component::Ptr>(m_panel)->relocate(rect.left + margin_.left, rect.getWidth() - margin_.left * 2, locationY);
}

// ----------------------------------------------------------------------------
void Frame<>::onDock()
{
    m_prevLocation = getLocation();
}

// ----------------------------------------------------------------------------
void Frame<>::onUndock()
{
    getLocation() = m_prevLocation;
    relocateChildren();
}

// ----------------------------------------------------------------------------
void Frame<>::drawDockable(Graphics& graphics)
{
    draw(graphics);
}

END_NAMESPACE_NYANCO_GUI
