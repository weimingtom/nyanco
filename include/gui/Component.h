#pragma once

/*!
    @file   Component.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <string>

namespace nyanco { namespace gui
{
    class Graphics;

    class Component
    {
    public:
        virtual void draw(Graphics& graphics) = 0;
        virtual Component* checkHit(int x, int y);

        void focus();
        bool isFocused();

    protected:
        std::string                     name_;
        Rect                            location_;
    };

} } // namespace nyanco::gui
