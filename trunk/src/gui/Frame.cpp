/*!
    @file   Frame.cpp
    @author dasyprocta
 */

#include "Frame.h"
#include "Graphics.h"

namespace nyanco { namespace gui
{

    void Frame::draw(
        Graphics&                       graphics)
    {
#if 0
        graphics.setColor(0xff333333);
        graphics.drawFillRect(location_);

        {
            Rect caption = location_;
            caption.left    += 3;
            caption.right   -= 3;
            caption.top     += 3;
            caption.bottom   = caption.top + 14;
            graphics.setColor(0xff6666ff);
            graphics.drawFillRect(caption);
        }
#endif
    }

    Component* Frame::checkHit(int x, int y)
    {
        // UNDONE:
        return 0;
    }

} } // namespace nyanco::gui
