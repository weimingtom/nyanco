/*!
    @file   Label.cpp
    @author dasyprocta
 */

#include "Label.h"
#include "Graphics.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Label::Ptr Label::Create(Arg<> const& arg, ComponentId id)
{
    Label* p = new Label;

    p->m_arg = arg;
    p->setLocation(Rect(0, 0, 0, 14));

    return Ptr(p);
}

// ----------------------------------------------------------------------------
void Label::draw(Graphics& graphics)
{
    Rect caption = location_;
    caption.bottom = caption.top + 14;
    Rect clip = getLocation();
    graphics.drawText(Point(caption.left+1, caption.top+1), m_arg.m_text, 0xffeeeeee, clip);
}

// ----------------------------------------------------------------------------
int Label::getHeight() const
{
    return 14;
}

END_NAMESPACE_NYANCO_GUI
