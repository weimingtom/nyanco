#pragma once

/*!
    @file   InputDevice.hpp
    @author dasyprocta
 */

#include "base.h"

namespace nyanco
{

    // ========================================================================
    class Keyboard
    {
    public:
        virtual bool onKeyDown(uint8 key) const = 0;
        virtual bool onKeyPush(uint8 key) const = 0;
        virtual bool onKeyUp(uint8 key) const = 0;
    };

    // ========================================================================
    class Mouse
    {
    public:
        virtual bool onMouseDown() const = 0;
        virtual bool onMousePush() const = 0;
        virtual bool onMouseUp() const = 0;

        virtual void getPosition(
            int&                        x,
            int&                        y) const = 0;

        virtual void getPreviousPosition(
            int&                        x,
            int&                        y) const = 0;
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

} // namespace nyanco

