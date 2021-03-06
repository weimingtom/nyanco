#pragma once

/*!
    @file   InputDevice.h
    @author dasyprocta
 */

#include "base.h"
#include "KeyCode.h"

BEGIN_NAMESPACE_NYANCO

// ========================================================================
class Keyboard
{
public:
    virtual bool onKeyDown(uint8 key) const = 0;
    virtual bool onKeyPush(uint8 key) const = 0;
    virtual bool onKeyUp(uint8 key) const = 0;

    virtual KeyCode::Type getKeyCode() const = 0;
    virtual char8 getAsciiCode() const = 0;
};

// ========================================================================
class Mouse
{
public:
    struct Button
    {
        enum Id
        {
            Left                    = 0,
            Right,
            Center,
        };
    };

    virtual bool onButtonDown(Button::Id button) const = 0;
    virtual bool onButtonPush(Button::Id button) const = 0;
    virtual bool onButtonUp(Button::Id button) const = 0;

    virtual void getPosition(
        int&                        x,
        int&                        y) const = 0;

    virtual void getPreviousPosition(
        int&                        x,
        int&                        y) const = 0;

    virtual KeyCode::Type getKeyCode() const = 0;
};

// ========================================================================
class InputDevice
{
public:
    virtual Keyboard& getKeyboard() const = 0;
    virtual Mouse& getMouse() const = 0;

#if 0
    virtual void getKeyboardCount() const = 0;
    virtual void getMouseCount() const = 0;
#endif

    static InputDevice& GetInterface();

    virtual ~InputDevice() {}
};

END_NAMESPACE_NYANCO
