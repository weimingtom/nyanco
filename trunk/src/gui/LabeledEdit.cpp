/*!
    @file   LabeledEdit.cpp
    @author dasyprocta
 */

#include "LabeledEdit.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
LabeledTextEdit::Ptr LabeledTextEdit::Create(Arg<> const& arg, ComponentId id)
{
    Ptr p(new LabeledTextEdit);

    p->m_label      = Label::Create(Label::Arg<>().text(arg.m_text));
    p->m_textEdit  = TextEdit::Create(-1);

    p->m_splitPanel->get<0>()->attach(p->m_label);
    p->m_splitPanel->get<1>()->attach(p->m_textEdit);
    p->m_splitPanel->setEventServer(p.get());

    p->getLocation() = Rect(0, 0, 0, p->m_splitPanel->getHeight());

    p->setId(id);

    return p;
}

// ----------------------------------------------------------------------------
void LabeledTextEdit::update()
{
    invokeHandler();
}

// ----------------------------------------------------------------------------
void LabeledTextEdit::draw(Graphics& graphics)
{
    m_splitPanel->draw(graphics);
}

// ----------------------------------------------------------------------------
int LabeledTextEdit::getHeight() const
{
    return m_splitPanel->getHeight();
}

// ----------------------------------------------------------------------------
sint32 LabeledTextEdit::relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY)
{
    Component::relocate(parentLeft, parentWidth, locationY);
    m_splitPanel->relocate(parentLeft, parentWidth, locationY);
    return getLocation().bottom;
}

// ----------------------------------------------------------------------------
void LabeledTextEdit::move(int x, int y)
{
    Component::move(x, y);
    m_splitPanel->move(x, y);
}

// ----------------------------------------------------------------------------
void LabeledTextEdit::focus()
{
    Component::focus();
    m_textEdit->focus();
}

// ----------------------------------------------------------------------------
void LabeledTextEdit::defocus()
{
    Component::defocus();
    m_textEdit->defocus();
}

// ----------------------------------------------------------------------------
bool LabeledTextEdit::onMouseProcess(MouseCommand const& command)
{
    if (m_textEdit->isPointInner(Point(command.posX, command.posY)))
    {
        return m_textEdit->onMouseProcess(command);
    }
    return false;
}

// ----------------------------------------------------------------------------
bool LabeledTextEdit::onKeyboardProcess(KeyboardCommand const& command)
{
    if (m_textEdit->isFocused())
    {
        return m_textEdit->onKeyboardProcess(command);
    }
    return false;
}


END_NAMESPACE_NYANCO_GUI
