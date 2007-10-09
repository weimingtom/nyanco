/*!
    @file   FontTexture.hpp
    @author dasyprocta
 */

#include "gui_base.h"
#include <d3dx9.h>

namespace nyanco { namespace gui { namespace impl
{

    struct FontInfo
    {
        uint32                              texWidth;
        uint32                              texHeight;
        uint32                              charaWidth;
        uint32                              charaHeight;
        uint32                              rowCount;
        uint32                              columnCount;
    };

    LPDIRECT3DTEXTURE9 createFontTexture(
        LPDIRECT3DDEVICE9               device,
        FontInfo&                       info);

} } } // namespace nyanco::impl
