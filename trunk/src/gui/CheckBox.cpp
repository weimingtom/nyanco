/*!
    @file   CheckBox.cpp
    @author dasyprocta
 */

#include "CheckBox.h"
#include "Graphics.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
CheckBoxPtr CheckBox::Create(
    std::string const&              name,
    std::string const&              text,
    bool                            check)
{
    CheckBox* p = new CheckBox;

    p->text_ = text;
    p->check_ = check;

    p->setName(name);
    p->setLocation(Rect(0, 0, 0, 16));

    return CheckBoxPtr(p);
}

// ----------------------------------------------------------------------------
void CheckBox::draw(Graphics& graphics)
{
    Rect box = location_;
    box.left += 2;
    box.top += 2;
    box.setWidth(12);
    box.setHeight(12);

    graphics.setColor(0xff333333);
    graphics.drawFillRect(box);

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
    graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
    graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));

    graphics.drawText(Point(location_.left+17, location_.top+2), text_, 0xffeeeeee);
}

// ----------------------------------------------------------------------------
int CheckBox::getHeight() const
{
    return 16;
}

END_NAMESPACE_NYANCO_GUI
