#pragma once

/*!
    @file   Graphics.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <d3d9.h>
#include <string>

namespace nyanco { namespace gui
{

    class Graphics
    {
    public:
        virtual void setColor(
            Color                       color) = 0;

        virtual void drawText(
            Point const&                point,
            std::string const&          text,
            Color                       color) = 0;

        virtual void drawRect(
            Rect const&                 rect) = 0;

        virtual void drawFillRect(
            Rect const&                 rect) = 0;
    };

} } // namespace nyanco::gui
