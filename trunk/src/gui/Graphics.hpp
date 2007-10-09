#pragma once

/*!
    @file   Graphics.hpp
    @author dasyprocta
 */

#include "gui_base.h"
#include "gui/Graphics.h"
#include "FontTexture.hpp"
#include <d3d9.h>
#include <string>

namespace nyanco { namespace gui { namespace impl
{
    class Graphics : public nyanco::gui::Graphics
    {
    public:
        void setColor(
            Color                       color);

        void drawText(
            Point const&                point,
            std::string const&          text,
            Color                       color);

        void drawRect(
            Rect const&                 rect);

        void drawFillRect(
            Rect const&                 rect);

        Graphics(
            IDirect3DDevice9*           devicePtr);
        ~Graphics();

    private:
        Color                           color_;

        LPDIRECT3DTEXTURE9              fontTexture_;
        FontInfo                        fontInfo_;
        IDirect3DDevice9&               device_;
    };

} } } // namespace nyanco::gui::impl