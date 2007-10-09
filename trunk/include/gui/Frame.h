#pragma once

/*!
    @file   Frame.h
    @author dasyprocta
 */

#include "base.h"
#include "Container.h"

namespace nyanco { namespace gui
{
    class WindowManager;
    class Graphics;

    class Frame : public Container
    {
    public:
        void draw(Graphics& graphics);

    private:
        Component* checkHit(int x, int y);

        Component*                      focusedComponent_;

        friend WindowManager;
    };

} } // namespace nyanco::gui
