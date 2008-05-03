/*!
    @file   EditField.cpp
    @author dasyprocta
 */

#include "EditField.h"
#include "Graphics.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
TextField::Ptr TextField::Create(ComponentId id)
{
    TextField* p = new TextField;
    p->setLocation(Rect(0, 0, 0, p->getHeight()));
    return Ptr(p);
}

// ----------------------------------------------------------------------------
void TextField::draw(Graphics& graphics)
{
    Rect box = location_;
    box.left += 2; box.top += 2;
    box.right -= 2; box.bottom -= 2;

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
    graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
    graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));

    Rect clip = location_;
    clip.left += 2; clip.right -= 2;
    size_t textWidth = m_text.size() * 6;
    size_t left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point(box.left + left, box.top + 2), m_text, 0xffeeeeee, clip);

    if (isFocused())
    {
        if (m_timer.elapsed() > 1.0)
        {
            m_timer = boost::timer();
        }
        else if (m_timer.elapsed() < 0.5)
        {
            graphics.setColor(0xffeeeeee);
            graphics.drawLine(Point(box.left+2+(6*m_caret), box.top+2), Point(box.left+2+(6*m_caret), box.bottom-2));
        }
    }
}

// ----------------------------------------------------------------------------
bool TextField::onMouseProcess(MouseCommand const& command)
{
    if (command.onPushLeft)
    {
        setCaret(command.posX, command.posY);
        m_timer = boost::timer();
    }
    return false;
}

// ----------------------------------------------------------------------------
void TextField::onKeyboardProcess(KeyboardCommand const& command)
{
    if (isFocused())
    {
        
    }
}

// ----------------------------------------------------------------------------
int TextField::getHeight() const
{
    return 18;
}

// ----------------------------------------------------------------------------
void TextField::setCaret(sint32 x, sint32 y)
{
    sint32 leftOffset = x - (location_.left+2);
    m_caret = leftOffset / 6;
    if      (m_caret < 0) m_caret = 0;
    else if (m_caret > m_text.size()) m_caret = m_text.size();
}

END_NAMESPACE_NYANCO_GUI
