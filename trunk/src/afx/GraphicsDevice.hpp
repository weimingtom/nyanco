#pragma once

/*!
    @file   GraphicsDevice.hpp
    @author dasyprocta
 */

#include "nyanco.hpp"
#include "GraphicsDevice.h"

namespace nyanco { namespace impl
{

    class GraphicsDevice : public nyanco::GraphicsDevice
    {
    public:

        virtual nyanco::Direct3dDevice* getD3dDevice() const;

        virtual void setClearColor(uint32 color);

        //! create direct3d device
        bool create(
            HWND                        hwnd,
            int                         width,
            int                         height,
            bool                        fullscreen);

        //! release direct3d device
        void release();

        void clear();
        bool present();

        bool reset();
        bool checkReset();

        D3DPRESENT_PARAMETERS& getPresentParameters();

        static GraphicsDevice& GetImplement();

    private:
        GraphicsDevice() : clearColor_(0xff000088) {}

    private:
        LPDIRECT3D9                     d3d_;
        LPDIRECT3DDEVICE9               device_;
        D3DPRESENT_PARAMETERS           presentParameters_;

        // parameter
        uint32                          clearColor_;

        static GraphicsDevice*          My_;

        friend nyanco::GraphicsDevice;
    };

    inline D3DPRESENT_PARAMETERS& GraphicsDevice::getPresentParameters()
    {
        return presentParameters_;
    }

}} // namespace nyanco::impl
