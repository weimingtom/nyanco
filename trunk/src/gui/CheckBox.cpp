/*!
    @file   CheckBox.cpp
    @author dasyprocta
 */

#include "CheckBox.h"
#include "Graphics.hpp"
#include "Frame.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
CheckBox::Ptr CheckBox::Create(
    ComponentId                     id,
    std::string const&              text,
    bool                            check)
{
    CheckBox* p = new CheckBox;

    p->text_        = text;
    p->m_checked    = check;
    p->m_id         = id;
    p->setLocation(Rect<sint32>(0, 0, 0, 16));

    return CheckBox::Ptr(p);
}

// ----------------------------------------------------------------------------
void CheckBox::draw(Graphics& graphics)
{
    Rect<sint32> box = location_;
    box.left += 2;
    box.top += 2;
    box.setWidth(12);
    box.setHeight(12);

    graphics.setColor(0xff333333);
    graphics.drawFillRect(box);

    graphics.setColor(0xff222222);
    graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
    graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));

    graphics.setColor(0xff888888);
    graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
    graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));

    Rect<sint32> clip = location_;
    clip.right -= 2;
    graphics.drawText(Point<sint32>(location_.left+17, location_.top+2), text_, 0xffeeeeee, clip);

    if (m_checked)
    {
        graphics.setColor(0xffeeeeee);
        graphics.drawLine(Point<sint32>(box.left+2, box.top+2), Point<sint32>(box.right-1, box.bottom-1));
        graphics.drawLine(Point<sint32>(box.left+2, box.bottom-2), Point<sint32>(box.right-1, box.top+1));
    }
}

// ----------------------------------------------------------------------------
int CheckBox::getHeight() const
{
    return 16;
}

// ----------------------------------------------------------------------------
bool CheckBox::onMouseProcess(MouseCommand const& command)
{
    if (command.onPushLeft)
    {
        m_pushed = true;
    }
    else if (command.onUpLeft && m_pushed)
    {
        if (location_.isInnerPoint(command.posX, command.posY))
        {
            EventServer* es = getEventServer();
            if (es != 0)
            {
                Ptr this_ = boost::shared_dynamic_cast<CheckBox>(shared_from_this());
                NAMESPACE_NYANCO_GUI::Event<CheckBox> event(this_, Event::Click);
                es->queueEvent(getId(), event);
            }
            m_checked = !m_checked;
        }
        m_pushed = false;
    }
    return m_pushed;
}

END_NAMESPACE_NYANCO_GUI
