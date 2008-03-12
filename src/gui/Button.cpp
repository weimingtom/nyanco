/*!
    @file   Button.cpp
    @author dasyprocta
 */

#include "Button.h"
#include "Graphics.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
ButtonPtr Button::Create(
    std::string const&                  name,
    std::string const&                  caption)
{
    Button* button = new Button;

    button->setName(name);
    button->caption_    = caption;
    button->setLocation(Rect(0, 0, 0, 18));

    return ButtonPtr(button);
}

// ----------------------------------------------------------------------------
void Button::draw(Graphics& graphics)
{
    Rect box = location_;
    box.left    += 2;
    box.top     += 2;
    box.right   -= 2;
    box.bottom  -= 2;

    graphics.setColor(0xff444444);
    graphics.drawFillRect(box);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
    graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
    graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));

    size_t textWidth = caption_.size() * 8;
    size_t left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point(box.left + left, box.top + 2), caption_, 0xffeeeeee);
}

// ----------------------------------------------------------------------------
int Button::getHeight() const
{
    return 18;
}

END_NAMESPACE_NYANCO_GUI
