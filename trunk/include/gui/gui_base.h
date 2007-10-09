#pragma once

/*!
    @file   gui_base.h
    @author dasyprocta
 */

#include "base.h"

namespace nyanco { namespace gui
{

    typedef uint32                      Color;

    class Point
    {
    public:
        sint32 x, y;

        Point(sint32 x_, sint32 y_) : x(x_), y(y_) {}
    };

    class Size
    {
    public:
        int width, height;
    };

    class Rect
    {
    public:
        int left, top, right, bottom;
    };

} } // namespace nyanco::gui
