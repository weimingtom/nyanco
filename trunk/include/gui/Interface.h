#pragma once

/*!
    @file   Interface.h
    @author dasyprocta
 */

#include "afx/GuiInterface.h"

namespace nyanco { namespace gui
{

    class Interface : public GuiInterface
    {
    protected:
        void onInitialize();
        void onFinalize();
        void onUpdate();
        void onDraw();

        static GuiInterface* Create()
        {
            return new Interface;
        }

        friend GuiInterface;
    };

} } // namespace nyanco::gui
