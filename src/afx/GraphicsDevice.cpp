/*!
    @file   GraphicsDevice.cpp
    @author dasyprocta
 */

#include "GraphicsDevice.hpp"

namespace nyanco { namespace impl
{

    GraphicsDevice* GraphicsDevice::My_ = 0;

    // ------------------------------------------------------------------------
    nyanco::Direct3dDevice* GraphicsDevice::getD3dDevice() const
    {
        return device_;
    }

    // ------------------------------------------------------------------------
    void GraphicsDevice::setClearColor(uint32 color)
    {
        clearColor_ = color;
    }

    // ------------------------------------------------------------------------
    bool GraphicsDevice::create(
        HWND                            hwnd,
        int                             width,
        int                             height,
        bool                            fullscreen)
    {
        LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (d3d == 0)
        {
            return false;
        }

        D3DPRESENT_PARAMETERS presentParameters;
        ZeroMemory(&presentParameters, sizeof(D3DPRESENT_PARAMETERS));

        // D3DPRESENT_PARAMETERS 構造体初期化
        //PresentParameters.BackBufferWidth               = width;
        //PresentParameters.BackBufferHeight              = height;
        presentParameters.BackBufferCount               = 1;
        presentParameters.MultiSampleType               = D3DMULTISAMPLE_NONE;
        presentParameters.MultiSampleQuality            = 0;
        presentParameters.SwapEffect                    = D3DSWAPEFFECT_DISCARD;
        //PresentParameters.hDeviceWindow                 = hwnd;
        presentParameters.EnableAutoDepthStencil        = TRUE;
        presentParameters.AutoDepthStencilFormat        = D3DFMT_D16;
        presentParameters.Flags                         = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

        // present parameter 構造体初期化
        presentParameters.BackBufferWidth        = width;
        presentParameters.BackBufferHeight       = height;
        presentParameters.hDeviceWindow          = hwnd;

        if (fullscreen)
        {
            presentParameters.BackBufferFormat           = D3DFMT_UNKNOWN;
            presentParameters.Windowed                   = FALSE;
            presentParameters.FullScreen_RefreshRateInHz = 60;
            presentParameters.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
        }
        else
        {
            presentParameters.BackBufferFormat           = D3DFMT_UNKNOWN;
            presentParameters.Windowed                   = TRUE;
            presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
            presentParameters.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
        }

        LPDIRECT3DDEVICE9 device;
        d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            presentParameters.hDeviceWindow,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentParameters,
            &device);

        d3d_                            = d3d;
        device_                         = device;
        presentParameters_              = presentParameters;

        return true;
    }

    // ------------------------------------------------------------------------
    void GraphicsDevice::release()
    {
        nyanco::safeRelease(device_);
        nyanco::safeRelease(d3d_);
    }

    // ------------------------------------------------------------------------
    void GraphicsDevice::clear()
    {
        device_->Clear(
            0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            clearColor_, 1.f, 0);
    }

    // ------------------------------------------------------------------------
    bool GraphicsDevice::present()
    {
        if (FAILED(device_->Present(0, 0, 0, 0)))
        {
            return false;
        }
        return true;
    }

    // ------------------------------------------------------------------------
    bool GraphicsDevice::reset()
    {
        HRESULT hr = device_->Reset(&presentParameters_);
        return SUCCEEDED(hr)? true: false;
    }

    // ------------------------------------------------------------------------
    bool GraphicsDevice::checkReset()
    {
        HRESULT hr = device_->TestCooperativeLevel();
        if (FAILED(hr))
        {
            // 復元不可能
            if (hr == D3DERR_DEVICELOST)
            {
                return false;
            }
            // 復元可能
            if (hr == D3DERR_DEVICENOTRESET)
            {
                // UNDONE: ウィンドウモード時のフォーマット指定
            }
        }

        return true;
    }

    // ------------------------------------------------------------------------
    GraphicsDevice& GraphicsDevice::GetImplement()
    {
        namespace impl = nyanco::impl;

        if (impl::GraphicsDevice::My_ == 0)
        {
            impl::GraphicsDevice::My_ = new impl::GraphicsDevice;
        }
        return *impl::GraphicsDevice::My_;
    }

}} // namespace nyanco::impl

namespace nyanco
{

    // ------------------------------------------------------------------------
    GraphicsDevice& GraphicsDevice::GetInterface()
    {
        namespace impl = nyanco::impl;
        
        if (impl::GraphicsDevice::My_ == 0)
        {
            impl::GraphicsDevice::My_ = new impl::GraphicsDevice;
        }
        return *impl::GraphicsDevice::My_;
    }

    // ------------------------------------------------------------------------
    GraphicsDevice* GraphicsDevice::GetInterfacePtr()
    {
        namespace impl = nyanco::impl;

        if (impl::GraphicsDevice::My_ == 0)
        {
            impl::GraphicsDevice::My_ = new impl::GraphicsDevice;
        }
        return impl::GraphicsDevice::My_;
    }

}

