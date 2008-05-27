#pragma once

/*!
    @file   Graphics.hpp
    @author dasyprocta
 */

#include "gui_impl_base.hpp"
#include "gui/Graphics.h"
#include "FontTexture.hpp"
#include <d3d9.h>
#include <string>

BEGIN_NAMESPACE_NYANCO_GUI_IMPL

class Graphics : public nyanco::gui::Graphics
{
public:
    virtual void setColor(
        Color                       color);

    virtual void setRectColor(
        Color                       color);

    virtual void setRectColor(
        Color                       leftTop,
        Color                       rightTop,
        Color                       leftBottom,
        Color                       rightBottom);

    virtual void drawText(
        Point<sint32> const&                point,
        std::string const&          text,
        Color                       color,
        Rect<sint32> const&                 region);

    virtual void drawRect(
        Rect<sint32> const&                 rect);

    virtual void drawFillRect(
        Rect<sint32> const&                 rect);

    virtual void drawLine(
        Point<sint32> const&                p1,
        Point<sint32> const&                p2);

    virtual void drawIbeamCursor(
        Point<sint32> const&                p);

    virtual void drawTriangle(
        Point<sint32> const&                p1,
        Point<sint32> const&                p2,
        Point<sint32> const&                p3);

    FontInfo const& getFontInfo() const { return fontInfo_; }

    Graphics(
        IDirect3DDevice9*           devicePtr);
    ~Graphics();

private:
    Color                           color_;
    Color                           rectColor_[4];

    LPDIRECT3DTEXTURE9              fontTexture_;
    FontInfo                        fontInfo_;
    IDirect3DDevice9&               device_;
};

END_NAMESPACE_NYANCO_GUI_IMPL
