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
    button->setLocation(Rect<sint32>(0, 0, 0, button->getHeight()));

    return Button::Ptr(button);
}

// ----------------------------------------------------------------------------
void Button::draw(Graphics& graphics)
{
    Rect<sint32> box = location_;
    box.left    += 2;
    box.top     += 2;
    box.right   -= 2;
    box.bottom  -= 2;

    if (!pushed_)
    {
        graphics.setRectColor(0xff444444);
        graphics.drawFillRect(box);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
        graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));
    }
    else
    {
        graphics.setRectColor(0xff444444);
        graphics.drawFillRect(box);

        graphics.setColor(0xff222222);
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));

        graphics.setColor(0xff888888);
        graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
        graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));
    }

    Rect<sint32> clip = location_;
    clip.left += 2; clip.right -= 2;
    sint32 textWidth = static_cast<sint32>(caption_.size()) * 6;
    sint32 left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point<sint32>(box.left + left, box.top + 2), caption_, 0xffeeeeee, clip);

    if (isFocused())
    {
        graphics.setColor(0xffaaaaaa);
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));
        graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
        graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));
    }
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
    }
    else if (command.onUpLeft && pushed_)
    {
        pushed_ = false;
        if (location_.isInnerPoint(command.posX, command.posY))
        {
            Ptr this_ = boost::shared_static_cast<Button>(shared_from_this());
            NAMESPACE_NYANCO_GUI::Event<Button> event(this_, Event::Click);
            queueEvent(getId(), event);
        }
    }
    return pushed_;
}

// ----------------------------------------------------------------------------
bool Button::onKeyboardProcess(KeyboardCommand const& command)
{
    if (command.code == KeyCode::Return)
    {
        Ptr this_ = boost::shared_static_cast<Button>(shared_from_this());
        NAMESPACE_NYANCO_GUI::Event<Button> event(this_, Event::Click);
        queueEvent(getId(), event);
    }
    return false;
}

END_NAMESPACE_NYANCO_GUI
