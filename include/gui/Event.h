#pragma once

/*!
    @file   Event.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <map>
#include <boost/bind.hpp>
#include <boost/function.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class EventBase
{
public:
    struct Type
    {
        ComponentId                     id;
        sint32                          event;

        bool operator < (Type const& type) const
        {
            if (id < type.id) return true;
            return event < type.event;
        }

        bool operator > (Type const& type) const
        {
            if (id > type.id) return true;
            return event > type.event;
        }

        bool operator == (Type const& type) const
        {
            if (id == type.id) return true;
            return event == type.event;
        }

        Type(ComponentId const& id_, sint32 event_)
            : id(id_), event(event_) {}
    };
};

// ============================================================================
template <typename EventComponent_>
class Event : public EventBase
{
public:
    typedef EventComponent_             EventComponent;
    typedef boost::function<void ()>    Handler;
    typedef std::map<Type, Handler>     Map;

    void regist(Type const& type, Handler handler, EventComponent* p = 0);
    void invoke(Type const& type) const;

#if 0
    Event<EventComponent>& operator()(Type const& type, Handler handler, EventComponent* p = 0)
    {
        regist(type, handler, p);
        return *this;
    }
#endif

private:
    Map                                 m_map;
};

// ----------------------------------------------------------------------------
template <typename EventComponent_>
void Event<EventComponent_>::regist(
    Type const&                         type,
    Handler                             handler,
    EventComponent*                     p)
{
    m_map[type] = handler;
}

// ----------------------------------------------------------------------------
template <typename EventComponent_>
void Event<EventComponent_>::invoke(
    Type const&                         type) const
{
    Map::const_iterator it = m_map.find(type);
    if (it != m_map.end())
    {
        it->second();
    }
}

END_NAMESPACE_NYANCO_GUI
