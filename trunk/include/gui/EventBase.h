#pragma once

/*!
    @file   EventBase.h
    @author dasyprocta
 */

#include "Component.h"
#include <map>
#include <boost/function.hpp>

namespace nyanco { namespace gui
{

    class Event {};

    class MenuEventListner
    {
        virtual void callback(
            ComponentId const&          componentId);
    };

    class MenuEvent
    {
    public:
        typedef boost::function<void ()> HandlerType;

        void regist(
            ComponentId const&          componentId,
            HandlerType                 handler);

        void unregist(
            ComponentId const&          componentId);

        static HandlerType              NullHandler;

    private:
        typedef std::map<ComponentId, HandlerType> HandlerMap;
        HandlerMap                      handlerMap_;
    };

    class ActionEvent
    {
        
    };

    template <typename WindowType_, typename HandlerType_>
    class EventHandler
    {
        
    };

}} // namespace nyanco::gui
