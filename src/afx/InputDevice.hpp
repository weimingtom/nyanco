#pragma once

/*!
    @file   InputDevice.hpp
    @author dasyprocta
 */

#define DIRECTINPUT_VERSION 0x0800

#include "nyanco.hpp"
#include "InputDevice.h"
#include <dinput.h>
#include <vector>

namespace nyanco { namespace impl
{

    // ========================================================================
    class Keyboard : public nyanco::Keyboard
    {
    public:
        bool onKeyDown(uint8 key) const
        {
            return (buffer_[current_ * 256 + key] & 0x80) != 0;
        }

        bool onKeyPush(uint8 key) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_ * 256 + key] & 0x80) != 0) &&
                   ((buffer_[prev_ * 256 + key] & 0x80) == 0);
        }

        bool onKeyUp(uint8 key) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_ * 256 + key] & 0x80) == 0) &&
                   ((buffer_[prev_ * 256 + key] & 0x80) != 0);
        }

        void swap()
        {
            current_ = (current_ == 0? 1: 0);
            std::memset(&buffer_[current_ * 256], 0, sizeof(uint8) * 256);
        }

        void set(uint8 const* buffer)
        {
            std::memcpy(&buffer_[current_ * 256], buffer, sizeof(uint8) * 256);
        }

        Keyboard()
        {
            std::memset(buffer_, 0, sizeof(buffer_));
            current_ = 0;
        }

    private:
        uint8                           buffer_[512];
        uint32                          current_; // 0 or 1;
    };

    // ========================================================================
    class Mouse : public nyanco::Mouse
    {
    public:
        bool onMouseDown(Button::Id button) const
        {
            return (buffer_[current_].rgbButtons[button] & 0x80) != 0;
        }

        bool onMousePush(Button::Id button) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_].rgbButtons[button] & 0x80) != 0) &&
                   ((buffer_[prev_].rgbButtons[button] & 0x80) == 0);
        }

        bool onMouseUp(Button::Id button) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_].rgbButtons[button] & 0x80) == 0) &&
                   ((buffer_[prev_].rgbButtons[button] & 0x80) != 0);
        }

        void getPosition(
            int&                        x,
            int&                        y) const
        {
            x = buffer_[current_].lX;
            y = buffer_[current_].lY;
        }

        void getPreviousPosition(
            int&                        x,
            int&                        y) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            x = buffer_[prev_].lX;
            y = buffer_[prev_].lY;
        }

        void swap()
        {
            current_ = (current_ == 0? 1: 0);
            std::memset(&buffer_[current_], 0, sizeof(DIMOUSESTATE2));
        }

        void set(DIMOUSESTATE2 const& buffer)
        {
            std::memcpy(&buffer_[current_], &buffer, sizeof(DIMOUSESTATE2));
        }

        Mouse()
        {
            std::memset(buffer_, 0, sizeof(buffer_));
            current_ = 0;
        }

    private:
        DIMOUSESTATE2                   buffer_[2];
        uint32                          current_; // 0 or 1;
    };

    // ========================================================================
    class InputDevice : public nyanco::InputDevice
    {
    public:

        nyanco::Keyboard& getKeyboard() const;
        nyanco::Mouse& getMouse() const;

#if 0
        void getKeyboardCount() const;
        void getMouseCount() const;
#endif

        bool create(
            HINSTANCE                   hinstance,
            HWND                        hwnd);

        void release();

        bool createKeyboards();
        bool createMouses();
        bool createJoypads();

        void acquire();

        ~InputDevice() { release(); }

        static InputDevice& GetImplement();

    private:
        static BOOL CALLBACK EnumerateDevices(
            const DIDEVICEINSTANCE*     deviceInstance,
            void*                       context);

        static BOOL CALLBACK EnumerateKeyboard(
            const DIDEVICEINSTANCE*     deviceInstance,
            void*                       context);

        static BOOL CALLBACK EnumerateMouse(
            const DIDEVICEINSTANCE*     deviceInstance,
            void*                       context);

        static BOOL CALLBACK EnumerateJoypad(
            const DIDEVICEINSTANCE*     deviceInstance,
            void*                       context);

    private:
        LPDIRECTINPUT8                  dinput_;

        std::vector<LPDIRECTINPUTDEVICE8> keyboards_;
        std::vector<LPDIRECTINPUTDEVICE8> mouses_;
        std::vector<LPDIRECTINPUTDEVICE8> joypads_;

        HWND                            hwnd_;

        
        // HACK: 複数デバイスの取り扱い
        Keyboard*                       keyboardPtr_;
        Mouse*                          mousePtr_;

        static InputDevice*             MyPtr_;

        InputDevice() :
            keyboardPtr_(0),
            mousePtr_(0),
            dinput_(0),
            hwnd_(0) {}

        friend nyanco::InputDevice;
    };

} } // namespace nyanco::impl
