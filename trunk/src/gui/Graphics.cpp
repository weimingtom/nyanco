/*!
    @file   Graphics.cpp
    @author dasyprocta
 */

#include "Graphics.hpp"

namespace
{
    struct FontVertex
    {
        nyanco::float32                 x, y, z;
        nyanco::float32                 rhw;
        nyanco::uint32                  color;
        nyanco::float32                 tu, tv;

        static DWORD const              Fvf;
    };

    DWORD const FontVertex::Fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    struct GuiVertex
    {
        nyanco::float32                 x, y, z;
        nyanco::float32                 rhw;
        nyanco::uint32                  color;

        static DWORD const              Fvf;
    };

    DWORD const GuiVertex::Fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

} // no namespace

BEGIN_NAMESPACE_NYANCO_GUI_IMPL

// ------------------------------------------------------------------------
void Graphics::setColor(
    Color                           color)
{
    color_ = color;
}

// ----------------------------------------------------------------------------
void Graphics::setRectColor(
    Color                           color)
{
    rectColor_[0] = rectColor_[1] = rectColor_[2] = rectColor_[3] = color;
}

// ----------------------------------------------------------------------------
void Graphics::setRectColor(
    Color                           leftTop,
    Color                           rightTop,
    Color                           leftBottom,
    Color                           rightBottom)
{
    rectColor_[0] = leftTop;
    rectColor_[1] = rightTop;
    rectColor_[2] = leftBottom;
    rectColor_[3] = rightBottom;
}

// ------------------------------------------------------------------------
void Graphics::drawText(
    Point const&                    point,
    std::string const&              text,
    Color                           color,
    Rect const&                     region)
{
    static float du = ((float)fontInfo_.charaWidth) / ((float)fontInfo_.texWidth);
    static float dv = ((float)fontInfo_.charaHeight) / ((float)fontInfo_.texHeight);

    uint32 vsize = text.size() * 4;
    FontVertex* v = new FontVertex[vsize];
    std::memset(v, 0, sizeof(FontVertex) * vsize);

    uint32 isize = text.size() * 6;
    uint16* indices = new uint16[isize];
    for (int i = 0, j = 0; i < isize; i += 6, j += 4)
    {
        indices[i]   = j;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j;
        indices[i+4] = j+2;
        indices[i+5] = j+3;
    }

    for (int i = 0, count = 0; i < vsize; i += 4, count++)
    {
        uint8 code = text[count] - 0x21;

        v[i].rhw = v[i+1].rhw = v[i+2].rhw = v[i+3].rhw = 1.f;
        v[i].color = v[i+1].color = v[i+2].color = v[i+3].color = color;
        v[i].x    = (point.x) + (count * fontInfo_.charaWidth);
        v[i].y    = (point.y);
        v[i].tu   = (float)(code % 16 * fontInfo_.charaWidth + 0.5f) / fontInfo_.texWidth;//(code % 16) * du;
        v[i].tv   = (float)(code / 16 * fontInfo_.charaHeight + 0.5f) / fontInfo_.texHeight;//(code / 16) * dv;

        // スペース入れ忘れのためアドホック
        if (text[count] == 0x20)
        {
            v[i].tu = 1.f - du;
            v[i].tv = 1.f - dv;
        }
        v[i+1].x  = v[i].x + fontInfo_.charaWidth;
        v[i+1].y  = v[i].y;
        v[i+1].tu = v[i].tu + du;
        v[i+1].tv = v[i].tv;
        v[i+2].x  = v[i].x + fontInfo_.charaWidth;
        v[i+2].y  = v[i].y + fontInfo_.charaHeight;
        v[i+2].tu = v[i].tu + du;
        v[i+2].tv = v[i].tv + dv;
        v[i+3].x  = v[i].x;
        v[i+3].y  = v[i].y + fontInfo_.charaHeight;
        v[i+3].tu = v[i].tu;
        v[i+3].tv = v[i].tv + dv;
    }

    device_.SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    device_.SetRenderState(D3DRS_ALPHAREF, 0x08);
    device_.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    device_.SetFVF(FontVertex::Fvf);
    device_.SetTexture(0, fontTexture_);

    D3DVIEWPORT9 viewport, clipView;
    device_.GetViewport(&viewport);

    clipView.X = region.left < 0? 0: region.left;
    clipView.Y = region.top < 0? 0: region.top;
    clipView.Width = region.getWidth();
    clipView.Height = region.getHeight();
    clipView.MinZ = 0.0f;
    clipView.MaxZ = 1.0f;
    device_.SetViewport(&clipView);
    
    device_.DrawIndexedPrimitiveUP(
        D3DPT_TRIANGLELIST,
        0,
        vsize,
        text.size() * 2,
        indices,
        D3DFMT_INDEX16,
        v,
        sizeof(FontVertex));
    device_.SetTexture(0, 0);

    device_.SetViewport(&viewport);
    device_.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    delete[] v;
    delete[] indices;
}

// ------------------------------------------------------------------------
void Graphics::drawRect(
    Rect const&                     rect)
{
    GuiVertex v[4] =
    {
        { rect.left,    rect.top,       0.f,    1.f },
        { rect.right,   rect.top,       0.f,    1.f },
        { rect.right,   rect.bottom,    0.f,    1.f },
        { rect.left,    rect.bottom,    0.f,    1.f }
    };

    // device.DrawPrimitiveUP;
}

// ------------------------------------------------------------------------
void Graphics::drawFillRect(
    Rect const&                     rect)
{
    GuiVertex v[4] =
    {
        { rect.left,    rect.top,       0.f,    1.f,    rectColor_[0] },
        { rect.right,   rect.top,       0.f,    1.f,    rectColor_[1] },
        { rect.right,   rect.bottom,    0.f,    1.f,    rectColor_[3] },
        { rect.left,    rect.bottom,    0.f,    1.f,    rectColor_[2] }
    };

    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_TRIANGLEFAN,
        2,
        v,
        sizeof(GuiVertex));
}

// ------------------------------------------------------------------------
void Graphics::drawLine(
    Point const&                    p1,
    Point const&                    p2)
{
    GuiVertex v[2] =
    {
        { p1.x,     p1.y,   0.f,    1.f,    color_ },
        { p2.x,     p2.y,   0.f,    1.f,    color_ },
    };

    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v,
        sizeof(GuiVertex));
}

// ------------------------------------------------------------------------
Graphics::Graphics(
    IDirect3DDevice9*               devicePtr)
    : device_(*devicePtr),
      color_(0xffffffff)
{
    fontTexture_ = createFontTexture(devicePtr, fontInfo_);
}

// ------------------------------------------------------------------------
Graphics::~Graphics()
{
    fontTexture_->Release();
}

END_NAMESPACE_NYANCO_GUI_IMPL
