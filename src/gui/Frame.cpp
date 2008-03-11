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
FramePtr Frame::Create(
    std::string const&              name,
    std::string const&              caption,
    uint32                          width,
    uint32                          height)
{
    Frame* frame = new Frame;
    frame->name_                    = name;
    frame->caption_                 = caption;

    {
        frame->location_.left = frame->location_.top = 0;
        frame->location_.right          = width;
        frame->location_.bottom         = height;
    }

    {
        frame->margin_.left = frame->margin_.right = 3;
        frame->margin_.top  = frame->margin_.bottom = 3;
    }

    FramePtr framePtr(frame);
    frame->attach(TitleBar::Create("", caption, framePtr));

    return framePtr;
}

// ----------------------------------------------------------------------------
void Frame::moveTo(int x, int y)
{
    location_.right     = x + location_.getWidth();
    location_.bottom    = y + location_.getHeight();
    location_.left      = x;
    location_.top       = y;
}

// ----------------------------------------------------------------------------
void Frame::move(int x, int y)
{
    location_.left  += x;
    location_.right += x;
    location_.top   += y;
    location_.bottom += y;

    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::move, _1, x, y));
}

// ----------------------------------------------------------------------------
void Frame::draw(
    Graphics&                       graphics)
{
    graphics.setColor(0xff444444);
    graphics.drawFillRect(location_);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.right, location_.top));
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.left, location_.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(location_.right, location_.top+1), Point(location_.right, location_.bottom));
    graphics.drawLine(Point(location_.left, location_.bottom), Point(location_.right, location_.bottom));

    // 子の描画
    using boost::bind;
    using boost::ref;
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
void Frame::update()
{
    relocateY();
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::update, _1));
}

// ----------------------------------------------------------------------------
void Frame::relocateY()
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
ComponentPtr Frame::getHitComponent(int x, int y)
{
    if (isPointInner(Point(x, y)))
    {
        // 子コンポーネントにヒット
        ComponentPtr hit = checkHit(x, y);
        if (hit != 0) return hit;
    }
    return ComponentPtr();
}

END_NAMESPACE_NYANCO_GUI
