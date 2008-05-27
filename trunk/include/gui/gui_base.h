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
    BOOST_PP_TUPLE_ELEM(3, 0, Seq_) BOOST_PP_CAT(m_, BOOST_PP_TUPLE_ELEM(3, 1, Seq_)); \
    typename Class_::Arg<Class_>& BOOST_PP_TUPLE_ELEM(3, 1, Seq_) BOOST_PP_LPAREN() boost::call_traits<BOOST_PP_TUPLE_ELEM(3, 0, Seq_)>::param_type param BOOST_PP_RPAREN() \
    { BOOST_PP_CAT(m_, BOOST_PP_TUPLE_ELEM(3, 1, Seq_)) = param; return *static_cast<typename Class_::Arg<Class_>*>(this); }

#define NYANCO_GUI_INIT_PARAM(Depth_, Class_, Seq_)             \
    BOOST_PP_CAT(m_, BOOST_PP_TUPLE_ELEM(3, 1, Seq_)) = BOOST_PP_TUPLE_ELEM(3, 2, Seq_);

#define NYANCO_GUI_ARG_PARAMS(Seq_)                             \
    BOOST_PP_SEQ_FOR_EACH(NYANCO_GUI_MAKE_PARAM, T_, Seq_);     \
    Arg() { BOOST_PP_SEQ_FOR_EACH(NYANCO_GUI_INIT_PARAM, dummy, Seq_) };

BEGIN_NAMESPACE_NYANCO_GUI

typedef uint32                      Color;
typedef sint32                      WindowId;
typedef sint32                      ComponentId;
typedef sint32                      WindowId;
sint32 const                        NonspecificId = -1;

// ----------------------------------------------------------------------------
template <typename T_>
class Point
{
public:
    typedef T_ ValueType;

    ValueType x, y;

    Point(ValueType x_, ValueType y_) : x(x_), y(y_) {}

    template <typename Dest_>
    Point<Dest_> to() const
    {
        return Point<Dest_>(static_cast<Dest_>(x), static_cast<Dest_>(y));
    }
};

// ----------------------------------------------------------------------------
class Size
{
public:
    int width, height;
};

// ----------------------------------------------------------------------------
template <typename T_>
class Rect
{
public:
    typedef T_ ValueType;

    ValueType left, top, right, bottom;

    Rect() {}

    Rect(ValueType left_, ValueType top_, ValueType right_, ValueType bottom_)
        : left(left_), top(top_), right(right_), bottom(bottom_) {}

    void setWidth(ValueType width) { right = left + width; }
    void setHeight(ValueType height) { bottom = top + height; }
    void setLeft(ValueType left_) { right = left_ + getWidth(); left = left_; }
    void setTop(ValueType top_) { bottom = top_ + getHeight(); top = top_; }

    void extend(ValueType size) { left -= size; top -= size; right += size; bottom += size; }

    ValueType getWidth() const { return right - left; }
    ValueType getHeight() const { return bottom - top; }
    void getSize(Size& size) const { size.width = getWidth(); size.height = getHeight(); }
    bool isInnerPoint(ValueType x, ValueType y)
    {
        return (left <= x && x <= right && top  <= y && y <= bottom);
    }

    template <typename Dest_>
    Rect<Dest_> to() const
    {
        return Rect<Dest_>(static_cast<Dest_>(left), static_cast<Dest_>(top), static_cast<Dest_>(right), static_cast<Dest_>(bottom));
    }
};

// ----------------------------------------------------------------------------
class ColorRect : public Rect<sint32>
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
