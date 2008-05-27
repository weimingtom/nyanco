/*!
    @file   EditField.cpp
    @author dasyprocta
 */

#include "EditField.h"
#include "Graphics.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
TextEdit::Ptr TextEdit::Create(ComponentId id)
{
    TextEdit* p = new TextEdit;
    p->setLocation(Rect<sint32>(0, 0, 0, p->getHeight()));
    return Ptr(p);
}

// ----------------------------------------------------------------------------
void TextEdit::draw(Graphics& graphics)
{
    ComponentGraphics g(graphics);

    Rect<sint32> box = location_;
    box.left += 2; box.top += 2;
    box.right -= 2; box.bottom -= 2;

    g.drawEdit(box);

    Rect<sint32> clip = location_;
    clip.left += 2; clip.right -= 2;
    graphics.drawText(Point<sint32>(box.left+2, box.top+2), m_text, 0xffeeeeee, clip);

    if (isFocused())
    {
        // キャレット
        if (m_timer.elapsed() > 1.0)
        {
            m_timer = boost::timer();
        }
        else if (m_timer.elapsed() < 0.5)
        {
            graphics.setColor(0xffeeeeee);
            graphics.drawLine(Point<sint32>(box.left+2+(6*m_caret), box.top+2), Point<sint32>(box.left+2+(6*m_caret), box.bottom-2));
        }
        // フォーカス
        graphics.setColor(0xffaaaaaa);
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
        graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));
        graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
        graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));
    }
}

// ----------------------------------------------------------------------------
bool TextEdit::onMouseProcess(MouseCommand const& command)
{
    if (location_.isInnerPoint(command.posX, command.posY) && command.onPushLeft)
    {
        setCaret(command.posX, command.posY);
        m_timer = boost::timer();
    }
    return false;
}

// ----------------------------------------------------------------------------
bool TextEdit::onKeyboardProcess(KeyboardCommand const& command)
{
    if (!isFocused()) return false;

    if (command.code == KeyCode::Left)
    {
        if (--m_caret < 0) m_caret = 0;
        m_timer = boost::timer();
    }
    else if (command.code == KeyCode::Right)
    {
        sint32 size = static_cast<sint32>(m_text.size());
        if (++m_caret > size) m_caret = size;
        m_timer = boost::timer();
    }
    else if (command.code == KeyCode::BackSpace)
    {
        if (m_caret == 0) return false;
        m_text.erase(--m_caret, 1);
        m_timer = boost::timer();
    }
    else if (command.code == KeyCode::Delete)
    {
        if (m_caret > static_cast<sint32>(m_text.size())) return false;
        m_text.erase(m_caret, 1);
        m_timer = boost::timer();
    }
    else if (command.code == KeyCode::Return)
    {
        
    }
    else if (0x20 <= command.ascii && command.ascii < 0x7F)
    {
        m_text.insert(m_caret++, 1, command.ascii);
        m_timer = boost::timer();
    }
    return false;
}

// ----------------------------------------------------------------------------
int TextEdit::getHeight() const
{
    return 18;
}

// ----------------------------------------------------------------------------
void TextEdit::setCaret(sint32 x, sint32 y)
{
    sint32 leftOffset = x - (location_.left+2);
    m_caret = leftOffset / 6;
    sint32 size = static_cast<sint32>(m_text.size());
    if      (m_caret < 0) m_caret = 0;
    else if (m_caret > size) m_caret = size;
}

END_NAMESPACE_NYANCO_GUI
