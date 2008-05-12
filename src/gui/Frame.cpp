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
    frame->m_id                     = id;

    m_titleBar = TitleBar::Create(-1, caption, frame);
    m_titleBar->setCaption(caption);

    m_panel->attachParent(frame);

    {
        frame->location_.left = frame->location_.top = 0;
        frame->location_.right          = width;
        frame->location_.bottom         = height;
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
    location_.right     = x + location_.getWidth();
    location_.bottom    = y + location_.getHeight();
    location_.left      = x;
    location_.top       = y;
}

// ----------------------------------------------------------------------------
void Frame<>::move(int x, int y)
{
    location_.left  += x;
    location_.right += x;
    location_.top   += y;
    location_.bottom += y;

    m_titleBar->move(x, y);
    static_cast<Component::Ptr>(m_panel)->move(x, y);
    //std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::move, _1, x, y));
}

// ----------------------------------------------------------------------------
void Frame<>::focus(sint32 componentId)
{
    focus(searchById(componentId));
}

// ----------------------------------------------------------------------------
void Frame<>::focus(Component::Ptr component)
{
    if (Component::Ptr comp = m_focusedComponent.lock()) comp->defocus();
    m_focusedComponent = component;
    if (Component::Ptr comp = m_focusedComponent.lock()) comp->focus();
}

// ----------------------------------------------------------------------------
void Frame<>::defocus()
{
    if (Component::Ptr comp = m_focusedComponent.lock()) comp->defocus();
    m_focusedComponent.reset();
}

// ----------------------------------------------------------------------------
Component::Ptr Frame<>::getFocusedComponent()
{
    return m_focusedComponent.lock();
}

// ----------------------------------------------------------------------------
void Frame<>::draw(
    Graphics&                       graphics)
{
    graphics.setRectColor(0xcc444444);
    graphics.drawFillRect(location_);

    graphics.setColor(0xcc888888);
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.right, location_.top));
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.left, location_.bottom-1));

    graphics.setColor(0xcc222222);
    graphics.drawLine(Point(location_.right, location_.top+1), Point(location_.right, location_.bottom));
    graphics.drawLine(Point(location_.left, location_.bottom), Point(location_.right, location_.bottom));

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
    // title bar
    int locationY = location_.top + margin_.top;
    m_titleBar->relocate(location_.left + margin_.left, location_.getWidth() - margin_.left * 2, locationY);
    // component
    locationY += static_cast<Component::Ptr>(m_titleBar)->getHeight() + margin_.top;
    static_cast<Component::Ptr>(m_panel)->relocate(location_.left + margin_.left, location_.getWidth() - margin_.left * 2, locationY);

    location_.bottom = locationY + static_cast<Component::Ptr>(m_panel)->getHeight() + margin_.bottom;;
}

#if 0
// ----------------------------------------------------------------------------
void Frame<>::relocateY()
{
    int locationY = location_.top + margin_.top;
    foreach (ComponentPtr p, componentList_)
    {
        int height = p->getHeight();
        p->setY(locationY);
        locationY += height + margin_.top;
    }
    location_.bottom = locationY + margin_.bottom;
}
#endif

// ----------------------------------------------------------------------------
ComponentPtr Frame<>::getHitComponent(int x, int y)
{
    if (isPointInner(Point(x, y)))
    {
        // title bar
        if (m_titleBar->isPointInner(Point(x, y)))
            return m_titleBar;

        // 子コンポーネントにヒット
        Component::Ptr hit = static_cast<Component::Ptr>(m_panel)->checkHit(x, y);
        if (hit != 0) return hit;
    }
    return ComponentPtr();
}

// ----------------------------------------------------------------------------
void Frame<>::getDockableRect(Rect& rect)
{
    rect = location_;
}

// ----------------------------------------------------------------------------
void Frame<>::setDockableRect(Rect const& rect)
{
    location_ = rect;
    // title bar
    int locationY = location_.top + margin_.top;
    m_titleBar->relocate(location_.left + margin_.left, location_.getWidth() - margin_.left * 2, locationY);
    // component
    locationY += static_cast<Component::Ptr>(m_titleBar)->getHeight() + margin_.top;
    static_cast<Component::Ptr>(m_panel)->relocate(location_.left + margin_.left, location_.getWidth() - margin_.left * 2, locationY);
}

// ----------------------------------------------------------------------------
void Frame<>::onDock()
{
    m_prevLocation = location_;
}

// ----------------------------------------------------------------------------
void Frame<>::onUndock()
{
    location_ = m_prevLocation;
    relocateChildren();
}

// ----------------------------------------------------------------------------
void Frame<>::drawDockable(Graphics& graphics)
{
    draw(graphics);
}

END_NAMESPACE_NYANCO_GUI
