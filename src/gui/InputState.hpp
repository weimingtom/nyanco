#pragma once

/*!
    @file   InputState.hpp
    @author dasyprocta
 */

#include "afx/InputDevice.h"

namespace nyanco { namespace gui { namespace impl
{

    class MouseState
    {
    private:
        uint32                          clickInterval_;
        uint32                          doubleClickInterval_;
    };

}} // namespace nyanco::gui::impl
