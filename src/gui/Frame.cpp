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
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::move, _1, x, y));
}

// ----------------------------------------------------------------------------
void Frame<>::draw(
    Graphics&                       graphics)
{
    graphics.setRectColor(0xff444444);
    graphics.drawFillRect(location_);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.right, location_.top));
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.left, location_.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(location_.right, location_.top+1), Point(location_.right, location_.bottom));
    graphics.drawLine(Point(location_.left, location_.bottom), Point(location_.right, location_.bottom));

    // title bar
    static_cast<Component::Ptr>(m_titleBar)->draw(graphics);
    
    // 子の描画
    using boost::bind;
    using boost::ref;
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
void Frame<>::update()
{
    //relocateChildren();
    //static_cast<Component::Ptr>(m_titleBar)->update();
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::update, _1));
}

// ----------------------------------------------------------------------------
void Frame<>::relocateChildren()
{
    // title bar
    int locationY = location_.top + margin_.top;
    m_titleBar->setX(location_.left + margin_.left);
    m_titleBar->setY(locationY);
    m_titleBar->resize(location_.getWidth() - margin_.left * 2);

    // component
    locationY += static_cast<Component::Ptr>(m_titleBar)->getHeight() + margin_.top;
    foreach (ComponentPtr p, componentList_)
    {
        int height = p->getHeight();
        p->setX(location_.left + margin_.left);
        p->setY(locationY);
        locationY += height + margin_.top;
    }
    location_.bottom = locationY;
    resize(location_.getWidth());
}

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
    location_.bottom = locationY;
}

// ----------------------------------------------------------------------------
ComponentPtr Frame<>::getHitComponent(int x, int y)
{
    if (isPointInner(Point(x, y)))
    {
        // title bar
        if (m_titleBar->isPointInner(Point(x, y)))
            return m_titleBar;

        // 子コンポーネントにヒット
        ComponentPtr hit = checkHit(x, y);
        if (hit != 0) return hit;
    }
    return ComponentPtr();
}

END_NAMESPACE_NYANCO_GUI
