#pragma once

/*!
    @file   KeyCode.hpp
    @author dasyprocta
 */

#include "KeyCode.h"

BEGIN_NAMESPACE_NYANCO

KeyCode::Type mapFromDirectInput(uint8 diCode);
KeyCode::Type mapFromVirtualKey(uint8 vkCode);

END_NAMESPACE_NYANCO
