#pragma once

/*!
    @file   GraphicsDevice.h
    @author dasyprocta
 */

#include <d3d9.h>

namespace nyanco
{

    typedef IDirect3D9                  Direct3d;
    typedef IDirect3DDevice9            Direct3dDevice;
    typedef D3DPRESENT_PARAMETERS       D3dPresentParameters;

    // ========================================================================
    class GraphicsDevice
    {
    public:
        virtual Direct3dDevice* getD3dDevice() const = 0;

        static GraphicsDevice& GetInterface();
        static GraphicsDevice* GetInterfacePtr();

    protected:
        GraphicsDevice() {}
        virtual ~GraphicsDevice() {}
    };

} // namespace nyanco
