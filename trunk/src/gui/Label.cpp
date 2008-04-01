/*!
    @file   Label.cpp
    @author dasyprocta
 */

#include "Label.h"
#include "Graphics.hpp"

namespace nyanco { namespace gui
{

    LabelPtr Label::Create(
        std::string const&              text)
    {
        Label* p = new Label;

        p->text_ = text;
        p->setLocation(Rect(0, 0, 0, 14));

        return LabelPtr(p);
    }

    void Label::draw(Graphics& graphics)
    {
        Rect caption = location_;
        caption.bottom = caption.top + 14;
        Rect clip = location_;
        graphics.drawText(Point(caption.left+1, caption.top+1), text_, 0xffeeeeee, clip);
    }

    int Label::getHeight() const
    {
        return 14;
    }

}} // namespace nyanco::gui
