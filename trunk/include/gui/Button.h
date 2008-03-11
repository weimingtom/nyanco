#pragma once

/*!
    @file   Button.h
    @author dasyprocta
 */

#include "Component.h"

namespace nyanco { namespace gui
{

    class Button
    {
    public:

    private:
        // override
        void draw();

        std::string                     caption_;
        bool                            pushed_;
    };

} } // namespace nyanco::gui
