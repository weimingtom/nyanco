#pragma once

/*!
    @file   Event.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <vector>
#include <map>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

template <typename Component_> class Event;
class EventServer;

// base event
template <>
class Event<void>
{
public:
    int getType() const { return m_type; }

protected:
    int                                 m_type;
};

// event type
template <typename Component_ = void>
class Event : public Event<void>
{
    typedef Component_ ComponentType;

public:
    ComponentType* getSender() { return m_sender; }

    Event(ComponentType* sender, int type)
        : m_sender(sender) { setType(type); }
    Event() {}

private:
    void setSender(ComponentType* sender) { m_sender = sender; }
    void setType(int type) { m_type = type; }

    ComponentType*                      m_sender;
    friend ComponentType;
};

// event server
class EventServer
{
    typedef boost::any                          AnyHandler;
    typedef void (*InvokerType)(void*, AnyHandler&, Event<> const&);
    typedef std::pair<AnyHandler, InvokerType>  HandlerSet;
    typedef std::map<ComponentId, HandlerSet>   HandlerMap;
    typedef std::pair<ComponentId, Event<> >    EventSet;
    typedef std::vector<EventSet>               EventQueue;

    template <typename Type_, typename Event_>
    struct Invoker
    {
        typedef Type_               Type;
        typedef Event<Event_>       EventType;
        typedef void (Type::*Handler)(EventType const&);
        //typedef boost::function<void (EventType const&)> Handler;
        static void Invoke(void* this_, AnyHandler& handler, Event<> const& event)
        {
            Handler&         h = boost::any_cast<Handler&>(handler);
            Type*            t = static_cast<Type*>(this_);
            EventType const& e = static_cast<EventType const&>(event);
            (t->*h)(e);
        }
    };

public:
    template <typename Type_, typename Event_>
    void registerHandler(int id, void (Type_::*handler)(Event<Event_> const&))
    {
        m_map[id] = HandlerSet(handler, &Invoker<Type_, Event_>::Invoke);
    }

    void unregisterHandler(int id)
    {
        m_map.erase(id);
    }

    template <typename Event_>
    void queueEvent(int id, Event<Event_> const& e)
    {
        m_queue.push_back(EventSet(id, e));
    }

    void invokeHandler()
    {
        BOOST_FOREACH(EventSet& e, m_queue)
        {
            HandlerMap::const_iterator it = m_map.find(e.first);
            if (it != m_map.end())
            {
                HandlerSet& hs = m_map[e.first];
                hs.second(this, hs.first, e.second);
            }
            //else assert(0);
        }
        m_queue.clear();
    }

private:
    EventQueue                          m_queue;
    HandlerMap                          m_map;
};

// event client
class EventClient
{
    
};

END_NAMESPACE_NYANCO_GUI
