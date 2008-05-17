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
        Point const&                point,
        std::string const&          text,
        Color                       color,
        Rect const&                 region) = 0;

    virtual void drawRect(
        Rect const&                 rect) = 0;

    virtual void drawFillRect(
        Rect const&                 rect) = 0;

    virtual void drawLine(
        Point const&                p1,
        Point const&                p2) = 0;

    virtual void drawIbeamCursor(
        Point const&                p) = 0;

    virtual void drawTriangle(
        Point const&                p1,
        Point const&                p2,
        Point const&                p3) = 0;
};

class ComponentGraphics
{
public:
    ComponentGraphics(Graphics& g) : m_g(g) {}

    void drawEdit(Rect const& rect);
    void drawButton(Rect const& rect, bool pushed);
    void drawFrame(Rect const& rect, bool rise = true, bool gradation = false);

private:
    Graphics&                           m_g;
};

END_NAMESPACE_NYANCO_GUI
