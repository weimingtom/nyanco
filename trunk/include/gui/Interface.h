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
        void setClientRect(sint32 left, sint32 top, sint32 right, sint32 bottom);
        void getClientRect(sint32& left, sint32& top, sint32& right, sint32& bottom);

        static GuiInterface* Create()
        {
            return new Interface;
        }

        friend GuiInterface;
    };

} } // namespace nyanco::gui
