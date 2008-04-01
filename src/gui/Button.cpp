/*!
    @file   Button.cpp
    @author dasyprocta
 */

#include "Button.h"
#include "Graphics.hpp"
#include "Event.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Button::Ptr Button::Create(
    ComponentId                         id,
    std::string const&                  caption)
{
    Button* button = new Button;

    button->m_id        = id;
    button->caption_    = caption;
    button->setLocation(Rect(0, 0, 0, 18));

    return Button::Ptr(button);
}

// ----------------------------------------------------------------------------
void Button::draw(Graphics& graphics)
{
    Rect box = location_;
    box.left    += 2;
    box.top     += 2;
    box.right   -= 2;
    box.bottom  -= 2;

    if (!pushed_)
    {
        graphics.setColor(0xff444444);
        graphics.drawFillRect(box);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
        graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
        graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));
    }
    else
    {
        graphics.setColor(0xff444444);
        graphics.drawFillRect(box);

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
        graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
        graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));
    }

    Rect clip = location_;
    clip.left += 2; clip.right -= 2;
    size_t textWidth = caption_.size() * 6;
    size_t left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point(box.left + left, box.top + 2), caption_, 0xffeeeeee, clip);
}

// ----------------------------------------------------------------------------
int Button::getHeight() const
{
    return 18;
}

// ----------------------------------------------------------------------------
bool Button::onMouseProcess(MouseCommand const& command)
{
    if (command.onPushLeft)
    {
        pushed_ = true;

        EventServer* es = getEventServer();
        if (es != 0)
        {
            Event<Button> event(this, DownEvent);
            es->queueEvent(m_id, event);
        }
    }
    else if (command.onUpLeft && pushed_)
    {
        pushed_ = false;
        if (location_.isInnerPoint(command.posX, command.posY))
        {
            EventServer* es = getEventServer();
            if (es != 0)
            {
                Event<Button> event(this, UpEvent);
                es->queueEvent(m_id, event);
            }
        }
    }
    return pushed_;
}

END_NAMESPACE_NYANCO_GUI
