#pragma once

/*!
    @file   gui_base.h
    @author dasyprocta
 */

#include "base.h"
#include "afx/InputDevice.h"
#include <string>
#include <boost/shared_ptr.hpp>

#define BEGIN_NAMESPACE_NYANCO_GUI namespace nyanco { namespace gui {
#define END_NAMESPACE_NYANCO_GUI }}

BEGIN_NAMESPACE_NYANCO_GUI

typedef uint32                      Color;
typedef sint32                      ComponentId;

// ----------------------------------------------------------------------------
class Point
{
public:
    sint32 x, y;

    Point(sint32 x_, sint32 y_) : x(x_), y(y_) {}
};

// ----------------------------------------------------------------------------
class Size
{
public:
    int width, height;
};

// ----------------------------------------------------------------------------
class Rect
{
public:
    int left, top, right, bottom;

    Rect() {}

    Rect(int left_, int top_, int right_, int bottom_)
        : left(left_), top(top_), right(right_), bottom(bottom_) {}

    void setWidth(int width) { right = left + width; }
    void setHeight(int height) { bottom = top + height; }
    int getWidth() const { return right - left; }
    int getHeight() const { return bottom - top; }
    bool isInnerPoint(int x, int y)
    {
        return (left <= x && x <= right && top  <= y && y <= bottom);
    }
};

// ----------------------------------------------------------------------------
class MouseCommand
{
public:
    bool                            onPushLeft;
    bool                            onDownLeft;
    bool                            onUpLeft;

    bool                            onPushRight;
    bool                            onDownRight;
    bool                            onUpRight;

    int                             posX;
    int                             posY;

    int                             moveX;
    int                             moveY;

    bool                            onButtonDown;

    static void Create(MouseCommand& command, Mouse const& mouse)
    {
        command.onPushLeft  = mouse.onButtonPush(Mouse::Button::Left);
        command.onDownLeft  = mouse.onButtonDown(Mouse::Button::Left);
        command.onUpLeft    = mouse.onButtonUp(Mouse::Button::Left);

        command.onPushRight  = mouse.onButtonPush(Mouse::Button::Right);
        command.onDownRight  = mouse.onButtonDown(Mouse::Button::Right);
        command.onUpRight    = mouse.onButtonUp(Mouse::Button::Right);

        mouse.getPosition(command.posX, command.posY);
        int x, y;
        mouse.getPreviousPosition(x, y);
        command.moveX = command.posX - x;
        command.moveY = command.posY - y;

        command.onButtonDown = command.onPushLeft || command.onPushRight ||
                               command.onDownLeft || command.onDownRight;
    }
};

class KeyboardCommand
{

};

END_NAMESPACE_NYANCO_GUI
