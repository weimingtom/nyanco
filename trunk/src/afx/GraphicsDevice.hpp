#pragma once

/*!
    @file   GraphicsDevice.hpp
    @author dasyprocta
 */

#include "GraphicsDevice.h"

namespace nyanco { namespace impl
{

    class GraphicsDevice : public nyanco::GraphicsDevice
    {
    public:

        virtual nyanco::Direct3dDevice* getD3dDevice() const;

        //! create direct3d device
        bool create(
            HWND                        hwnd,
            int                         width,
            int                         height,
            bool                        fullscreen);

        //! release direct3d device
        void release();

        static GraphicsDevice& GetImplement();

    private:
        LPDIRECT3D9                     d3d_;
        LPDIRECT3DDEVICE9               device_;
        D3DPRESENT_PARAMETERS           presentParameters_;

        static GraphicsDevice*          My_;

        friend nyanco::GraphicsDevice;
    };

}} // namespace nyanco::impl
