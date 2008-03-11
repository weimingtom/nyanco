/*!
    @file   Event.cpp
    @author dasyprocta
 */

#include "Event.h"

namespace nyanco { namespace gui
{

    void MenuEventListner::callback(
        ComponentId const&              componentId)
    {
        
    }

    MenuEvent::HandlerType MenuEvent::NullHandler;

    void MenuEvent::regist(
        ComponentId const&              componentId,
        HandlerType                     handler)
    {
        handlerMap_[componentId] = handler;
    }

    void MenuEvent::unregist(
        ComponentId const&              componentId)
    {
        
    }

}} // namespace nyanco::gui
