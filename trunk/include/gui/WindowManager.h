#pragma once

/*!
    @file   WindowManager.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Frame.h"
#include <d3d9.h>
#include <string>

namespace nyanco { namespace gui
{

    class Graphics;

    class WindowManager
    {
    public:
        virtual void drawText(
            Point const&                point,
            std::string const&          text,
            Color                       color) = 0;

        virtual void attach(
            FramePtr                    framePtr) = 0;

        virtual void detach(
            FramePtr                    framePtr) = 0;

        virtual void detach(
            std::string const&          name) = 0;

        virtual FramePtr search(
            std::string const&          name) = 0;

        virtual void activate(
            FramePtr                    framePtr) = 0;

        virtual FramePtr getActiveWindow() const = 0;

        static WindowManager& GetInterface();
    };

} } // namespace nyanco::gui
