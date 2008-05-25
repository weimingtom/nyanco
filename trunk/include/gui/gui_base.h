#pragma once

/*!
    @file   gui_base.h
    @author dasyprocta
 */

#include "base.h"
#include "afx/InputDevice.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/preprocessor.hpp>
#include <boost/call_traits.hpp>

#define BEGIN_NAMESPACE_NYANCO_GUI  namespace nyanco { namespace gui {
#define END_NAMESPACE_NYANCO_GUI    }}
#define NAMESPACE_NYANCO_GUI        nyanco::gui

#define NYANCO_GUI_COMPONENT_TYPEDEF(Component_)                \
    typedef boost::shared_ptr<Component_>       Ptr;            \
    typedef boost::shared_ptr<Component_ const> ConstPtr;       \
    typedef boost::weak_ptr<Component_>         WeakPtr;        \
    typedef boost::weak_ptr<Component_ const>   ConstWeakPtr;   \
    typedef boost::scoped_ptr<Component_>       ScopedPtr;

#define NYANCO_GUI_ARG_DEF_BEGIN(Name_)                         \
    template <typename T_ = Name_>                              \
    struct Arg                                                  \
    {

#define NYANCO_GUI_ARG_DEF_MIXIN1_BEGIN(Name_, Mix1_)           \
    template <typename T_ = Name_>                              \
    struct Arg : public Mix1_::Arg<T_>                          \
    {

#define NYANCO_GUI_ARG_DEF_MIXIN2_BEGIN(Name_, Mix1_, Mix2_)    \
    template <typename T_ = Name_>                              \
    struct Arg : public Mix1_::Arg<T_>, public Mix2_::Arg<T_>   \
    {

#define NYANCO_GUI_ARG_DEF_END(Name_)           };

#define NYANCO_GUI_MAKE_PARAM(Depth_, Class_, Seq_)             \
    BOOST_PP_TUPLE_ELEM(2, 0, Seq_) BOOST_PP_CAT(m_, BOOST_PP_TUPLE_ELEM(2, 1, Seq_)); \
    typename Class_::Arg<Class_>& BOOST_PP_TUPLE_ELEM(2, 1, Seq_) BOOST_PP_LPAREN() boost::call_traits<BOOST_PP_TUPLE_ELEM(2, 0, Seq_)>::param_type param BOOST_PP_RPAREN() \
    { BOOST_PP_CAT(m_, BOOST_PP_TUPLE_ELEM(2, 1, Seq_)) = param; return *static_cast<typename Class_::Arg<Class_>*>(this); }

#define NYANCO_GUI_ARG_PARAMS(Seq_) BOOST_PP_SEQ_FOR_EACH(NYANCO_GUI_MAKE_PARAM, T_, Seq_)

BEGIN_NAMESPACE_NYANCO_GUI

typedef uint32                      Color;
typedef sint32                      WindowId;
typedef sint32                      ComponentId;
typedef sint32                      WindowId;
sint32 const                        NonspecificId = -1;

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
    sint32 left, top, right, bottom;

    Rect() {}

    Rect(int left_, int top_, int right_, int bottom_)
        : left(left_), top(top_), right(right_), bottom(bottom_) {}

    void setWidth(int width) { right = left + width; }
    void setHeight(int height) { bottom = top + height; }
    void setLeft(int left_) { right = left_ + getWidth(); left = left_; }
    void setTop(int top_) { bottom = top_ + getHeight(); top = top_; }

    void extend(int size) { left -= size; top -= size; right += size; bottom += size; }

    int getWidth() const { return right - left; }
    int getHeight() const { return bottom - top; }
    void getSize(Size& size) const { size.width = getWidth(); size.height = getHeight(); }
    bool isInnerPoint(int x, int y)
    {
        return (left <= x && x <= right && top  <= y && y <= bottom);
    }
};

// ----------------------------------------------------------------------------
class ColorRect : public Rect
{
public:
    Color leftColor, topColor, rightColor, bottomColor;
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

    bool                            onButtonPush;
    bool                            onButtonDown;

    KeyCode::Type                   code;

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

        command.onButtonPush = command.onPushLeft || command.onPushRight;

        command.onButtonDown = command.onPushLeft || command.onPushRight ||
                               command.onDownLeft || command.onDownRight;

        command.code = mouse.getKeyCode();
    }
};

// ----------------------------------------------------------------------------
class KeyboardCommand
{
public:
    char8                               ascii;
    KeyCode::Type                       code;
    bool                                onAlt;
    bool                                onCtrl;
    bool                                onShift;

    static void Create(KeyboardCommand& command, Keyboard& keyboard)
    {
        command.ascii   = keyboard.getAsciiCode();
        command.code    = keyboard.getKeyCode();
    }
};

END_NAMESPACE_NYANCO_GUI
