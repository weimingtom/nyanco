#pragma once

/*!
    @file   InputDevice.hpp
    @author dasyprocta
 */

#define DIRECTINPUT_VERSION 0x0800

#include "nyanco.hpp"
#include "InputDevice.h"
#include "KeyCode.hpp"
#include <dinput.h>
#include <vector>
#include <deque>

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

        KeyCode::Type getKeyCode() const
        {
            return m_key;
        }

        char8 getAsciiCode() const
        {
            return static_cast<char8>(m_ascii);
        }

        void setVirtualKey(uint8 key)
        {
            m_key = mapFromVirtualKey(key);
        }

        void setAscii(sint8 ascii)
        {
            m_ascii = ascii;
        }

        void clear()
        {
            m_key   = KeyCode::Unknown;
            m_ascii = 0;
        }

#if 0
        char8 popAsciiCode()
        {
            if (m_asciiQueue.empty()) return -1;
            char8 elem = m_asciiQueue[0];
            m_asciiQueue.pop_front();
            return elem;
        }

        void pushAsciiCode(char8 ascii)
        {
            m_asciiQueue.push_back(ascii);
        }

        void clearAsciiQueue()
        {
            m_asciiQueue.clear();
        }
#endif

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

        KeyCode::Type                   m_key;
        sint8                           m_ascii;

//        std::deque<char8>               m_asciiQueue;
    };

    // ========================================================================
    class Mouse : public nyanco::Mouse
    {
    public:
        bool onButtonDown(Button::Id button) const
        {
            return (buffer_[current_].rgbButtons[button] & 0x80) != 0;
        }

        bool onButtonPush(Button::Id button) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_].rgbButtons[button] & 0x80) != 0) &&
                   ((buffer_[prev_].rgbButtons[button] & 0x80) == 0);
        }

        bool onButtonUp(Button::Id button) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            return ((buffer_[current_].rgbButtons[button] & 0x80) == 0) &&
                   ((buffer_[prev_].rgbButtons[button] & 0x80) != 0);
        }

        void getPosition(
            int&                        x,
            int&                        y) const
        {
            x = position_[current_].x;
            y = position_[current_].y;
        }

        void getPreviousPosition(
            int&                        x,
            int&                        y) const
        {
            uint32 const prev_ = (current_ == 0? 1: 0);
            x = position_[prev_].x;
            y = position_[prev_].y;
        }

        KeyCode::Type getKeyCode() const
        {
            return m_key;
        }

        void swap()
        {
            current_ = (current_ == 0? 1: 0);
            std::memset(&buffer_[current_], 0, sizeof(DIMOUSESTATE2));
            std::memset(&position_[current_], 0, sizeof(POINT));
        }

        void set(DIMOUSESTATE2 const& buffer, POINT const& position)
        {
            std::memcpy(&buffer_[current_], &buffer, sizeof(DIMOUSESTATE2));
            std::memcpy(&position_[current_], &position, sizeof(POINT));
        }

        void setVirtualKey(uint8 key)
        {
            m_key = mapFromVirtualKey(key);
        }

        void clear()
        {
            m_key = KeyCode::Unknown;
        }

        Mouse()
        {
            std::memset(buffer_, 0, sizeof(buffer_));
            std::memset(position_, 0, sizeof(position_));
            current_ = 0;
        }

    private:
        DIMOUSESTATE2                   buffer_[2];
        POINT                           position_[2];
        uint32                          current_; // 0 or 1;

        KeyCode::Type                   m_key;
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

        Keyboard& getImplKeyboard() const
        {
            return *keyboardPtr_;
        }

        Mouse& getImplMouse() const
        {
            return *mousePtr_;
        }

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
