/*!
    @file   InputDevice.cpp
    @author dasyprocta
 */

#include "InputDevice.hpp"
#include "../nyanco.hpp"

namespace nyanco { namespace impl
{

    InputDevice* InputDevice::MyPtr_ = 0;

    // ----------------------------------------------------------------------------
    nyanco::Keyboard& InputDevice::getKeyboard() const
    {
        return *keyboardPtr_;
    }

    // ----------------------------------------------------------------------------
    nyanco::Mouse& InputDevice::getMouse() const
    {
        return *mousePtr_;
    }

    // ------------------------------------------------------------------------
    bool InputDevice::create(
        HINSTANCE                       hinstance,
        HWND                            hwnd)
    {
        LPDIRECTINPUT8  dinput;
        HRESULT hr = DirectInput8Create(
            hinstance,
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            reinterpret_cast<void**>(&dinput),
            0);
        if (FAILED(hr)) return false;

        // initialize member
        hwnd_           = hwnd;
        dinput_         = dinput;
        keyboardPtr_    = new Keyboard;
        mousePtr_       = new Mouse;

        createKeyboards();
        createMouses();

        return true;
    }

    // ------------------------------------------------------------------------
    void InputDevice::release()
    {
        safeDelete(keyboardPtr_);
        safeDelete(mousePtr_);
        safeRelease(dinput_);
    }

    // ------------------------------------------------------------------------
    bool InputDevice::createKeyboards()
    {
        HRESULT hr = dinput_->EnumDevices(
            DI8DEVTYPE_KEYBOARD,
            EnumerateKeyboard,
            this,
            DIEDFL_ATTACHEDONLY);
        if (FAILED(hr)) return false;

        return true;
    }

    // ------------------------------------------------------------------------
    bool InputDevice::createMouses()
    {
        HRESULT hr = dinput_->EnumDevices(
            DI8DEVTYPE_MOUSE,
            EnumerateMouse,
            this,
            DIEDFL_ATTACHEDONLY);
        if (FAILED(hr)) return false;

        return true;
    }

    // ----------------------------------------------------------------------------
    bool InputDevice::createJoypads()
    {
        return false;
    }

    // ------------------------------------------------------------------------
    void InputDevice::acquire()
    {
        // keyboard
        {
            if (!keyboards_.empty())
            {
                keyboardPtr_->swap();

                // HACK: 複数キーボードのマージ
                HRESULT hr = keyboards_[0]->Acquire();
                if (hr == DI_OK || hr == S_FALSE)
                {
                    BYTE key[256];
                    keyboards_[0]->GetDeviceState(sizeof(key), &key);
                    keyboardPtr_->set(key);
                }
            }
        }

        // mouse
        {
            if (!mouses_.empty())
            {
                // HACK: 複数マウスのマージ
                HRESULT hr = mouses_[0]->Acquire();
                if (hr == DI_OK || hr == S_FALSE)
                {
                    DIMOUSESTATE2 mouse = { 0 };
                    mouses_[0]->GetDeviceState(sizeof(mouse), &mouse);
                    mousePtr_->set(mouse);
                }
            }
        }

        // UNDONE: joypad
    }

    // ------------------------------------------------------------------------
    BOOL CALLBACK InputDevice::EnumerateDevices(
            const DIDEVICEINSTANCE*     instance,
            void*                       context)
    {
        return DIENUM_CONTINUE;
    }

    // ------------------------------------------------------------------------
    BOOL CALLBACK InputDevice::EnumerateKeyboard(
        const DIDEVICEINSTANCE*         instance,
        void*                           context)
    {
        InputDevice* device = reinterpret_cast<InputDevice*>(context);

        LPDIRECTINPUT8          dinput      = device->dinput_;
        LPDIRECTINPUTDEVICE8    didevice;

        HRESULT hr = dinput->CreateDevice(instance->guidInstance, &didevice, 0);
        if (FAILED(hr)) return DIENUM_STOP;

        didevice->SetDataFormat(&c_dfDIKeyboard);
        didevice->SetCooperativeLevel(
            device->hwnd_,
            DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

        device->keyboards_.push_back(didevice);

        return DIENUM_CONTINUE;
    }

    // ------------------------------------------------------------------------
    BOOL CALLBACK InputDevice::EnumerateMouse(
        const DIDEVICEINSTANCE*         instance,
        void*                           context)
    {
        InputDevice* device = reinterpret_cast<InputDevice*>(context);

        LPDIRECTINPUT8          dinput      = device->dinput_;
        LPDIRECTINPUTDEVICE8    didevice;

        HRESULT hr = dinput->CreateDevice(instance->guidInstance, &didevice, 0);
        if (FAILED(hr)) return DIENUM_STOP;

        didevice->SetDataFormat(&c_dfDIMouse2);
        didevice->SetCooperativeLevel(
            device->hwnd_,
            DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

        DIPROPDWORD diprop;
        diprop.diph.dwSize          = sizeof(diprop);
        diprop.diph.dwHeaderSize    = sizeof(diprop.diph);
        diprop.diph.dwObj           = 0;
        diprop.diph.dwHow           = DIPH_DEVICE;
        diprop.dwData               = DIPROPAXISMODE_ABS;
        didevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

        device->mouses_.push_back(didevice);

        return DIENUM_CONTINUE;
    }

    // ------------------------------------------------------------------------
    BOOL CALLBACK InputDevice::EnumerateJoypad(
        const DIDEVICEINSTANCE*         instance,
        void*                           context)
    {
        InputDevice* p = reinterpret_cast<InputDevice*>(context);

        return TRUE;
    }

    // ------------------------------------------------------------------------
    InputDevice& InputDevice::GetImplement()
    {
        if (InputDevice::MyPtr_ == 0)
        {
            InputDevice::MyPtr_ = new InputDevice;
        }
        return *InputDevice::MyPtr_;
    }

} } // namespace nyanco::impl

namespace nyanco
{

    // ------------------------------------------------------------------------
    InputDevice& InputDevice::GetInterface()
    {
        if (impl::InputDevice::MyPtr_ == 0)
        {
            impl::InputDevice::MyPtr_ = new impl::InputDevice;
        }
        return *impl::InputDevice::MyPtr_;
    }

} // namespace nyanco
