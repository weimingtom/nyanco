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
#include <boost/tuple/tuple.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

template <typename Component_> class Event;
class EventServer;

// any event
template <>
class Event<void>
{
public:
    virtual ComponentId getSenderId() const { return -1; };

    template <typename Component_>
    typename Component_::Event* castTo() const
    {
        return dynamic_cast<typename Component_::Event*>(this);
    }
};

// component event
template <typename Component_ = void>
class Event : public Event<>
{
    typedef Component_                      ComponentType;
    typedef typename ComponentType::Ptr     ComponentPtr;
    typedef typename ComponentType::Event   ComponentEvent;
    typedef typename ComponentType::Event::Type EventType;

public:
    ComponentId     getSenderId() const
    {
        return m_sender->getId();
    }

    ComponentPtr    getSender() const   { return m_sender; }
    EventType       getType() const     { return m_type; }
    Event(ComponentPtr sender, EventType type)
        : m_sender(sender), m_type(type) {}
    Event() {}

private:
    void setSender(ComponentType* sender) { m_sender = sender; }
    void setType(int type) { m_type = type; }

    ComponentPtr                        m_sender;
    EventType                           m_type;

    friend ComponentType;
};

// event server
class EventServer
{
    typedef boost::any                          AnyHandler;
    typedef void (*InvokerType)(void*, AnyHandler&, Event<> const&);
    typedef boost::tuple<AnyHandler, InvokerType, void*>  HandlerSet;
    typedef std::map<ComponentId, HandlerSet>   HandlerMap;
    typedef std::pair<ComponentId, boost::shared_ptr<Event<> > >    EventSet;
    typedef std::vector<EventSet>               EventQueue;

    // member function
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
            EventType const& e = dynamic_cast<EventType const&>(event);
            (t->*h)(e);
        }
    };

    // function
    template <typename Event_>
    struct Invoker<void, Event_>
    {
        typedef Event<Event_>       EventType;
        typedef void (*Handler)(EventType const&);
        static void Invoke(void*, AnyHandler& handler, Event<> const& event)
        {
            Handler&         h = boost::any_cast<Handler&>(handler);
            EventType const& e = dynamic_cast<EventType const&>(event);
            (*h)(e);
        }
    };

public:
    typedef boost::shared_ptr<EventServer> Ptr;

    // register member function
    template <typename Type_, typename Event_>
    void registerHandler(int id, void (Type_::*handler)(Event<Event_> const&), Type_* this_)
    {
        m_map[id] = boost::make_tuple(handler, &Invoker<Type_, Event_>::Invoke, this_);
    }

    // register function
    template <typename Event_>
    void registerHandler(int id, void (*handler)(Event<Event_> const&))
    {
        m_map[id] = boost::make_tuple(handler, &Invoker<void, Event_>::Invoke, 0);
    }

    void unregisterHandler(int id)
    {
        m_map.erase(id);
    }

    template <typename Event_>
    void queueEvent(int id, Event<Event_> const& e)
    {
        boost::shared_ptr<Event<> > p(new Event<Event_>(e));
        m_queue.push_back(EventSet(id, p));
    }

    void invokeHandler()
    {
        BOOST_FOREACH(EventSet& e, m_queue)
        {
            HandlerMap::const_iterator it = m_map.find(e.first);
            if (it != m_map.end())
            {
                HandlerSet& hs = m_map[e.first];
                hs.get<1>()(hs.get<2>(), hs.get<0>(), *(e.second));
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
