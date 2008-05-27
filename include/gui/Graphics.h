#pragma once

/*!
    @file   Graphics.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <d3d9.h>
#include <string>

BEGIN_NAMESPACE_NYANCO_GUI

class Graphics
{
public:
    virtual void setColor(
        Color                       color) = 0;

    virtual void setRectColor(
        Color                       color) = 0;

    virtual void setRectColor(
        Color                       leftTop,
        Color                       rightTop,
        Color                       leftBottom,
        Color                       rightBottom) = 0;

    virtual void drawText(
        Point<sint32> const&                point,
        std::string const&          text,
        Color                       color,
        Rect<sint32> const&                 region) = 0;

    virtual void drawRect(
        Rect<sint32> const&                 rect) = 0;

    virtual void drawFillRect(
        Rect<sint32> const&                 rect) = 0;

    virtual void drawLine(
        Point<sint32> const&                p1,
        Point<sint32> const&                p2) = 0;

    virtual void drawIbeamCursor(
        Point<sint32> const&                p) = 0;

    virtual void drawTriangle(
        Point<sint32> const&                p1,
        Point<sint32> const&                p2,
        Point<sint32> const&                p3) = 0;
};

class ComponentGraphics
{
public:
    ComponentGraphics(Graphics& g) : m_g(g) {}

    void drawEdit(Rect<sint32> const& rect);
    void drawButton(Rect<sint32> const& rect, bool pushed);
    void drawFrame(Rect<sint32> const& rect, bool rise = true, bool gradation = false);

private:
    Graphics&                           m_g;
};

END_NAMESPACE_NYANCO_GUI
