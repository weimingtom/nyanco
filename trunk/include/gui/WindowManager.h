#pragma once

/*!
    @file   WindowManager.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <d3d9.h>
#include <string>

namespace nyanco { namespace gui
{

    class Graphics;
    class Frame;

    class WindowManager
    {
    public:
        virtual void drawText(
            Point const&                point,
            std::string const&          text,
            Color                       color) = 0;

        virtual void attach(
            Frame*                      framePtr) = 0;

        virtual void detach(
            Frame*                      framePtr) = 0;

        virtual void detach(
            std::string const&          name) = 0;

        virtual Frame* search(
            std::string const&          name) = 0;

        virtual Frame* getActiveWindow() const = 0;

        static WindowManager& GetInterface();
    };

} } // namespace nyanco::gui
