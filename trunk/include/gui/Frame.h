#pragma once

/*!
    @file   Frame.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Container.h"
#include "Event.h"
#include <vector>

BEGIN_NAMESPACE_NYANCO_GUI

class WindowManager;
class Graphics;

namespace impl { class WindowManager; }

class Frame;
template <typename Derived_> class Frame2;
typedef boost::shared_ptr<Frame>        FramePtr;

// ============================================================================
class Frame : public Container
{
public:
    void moveTo(int x, int y);
    void move(int x, int y);

    virtual void setEvent(EventBase::Type const& type) {}

protected:
    void create(
        FramePtr                        frame,
        ComponentId                     id,
        std::string const&              caption,
        uint32                          width,
        uint32                          height);
    void draw(Graphics& graphics);
    void update();

    virtual void onInitialize() {}
    virtual void invokeEvent() {}

private:
    void relocateY();
    ComponentPtr getHitComponent(int x, int y);

private:
    ComponentPtr                        focusedComponent_;
    std::string                         caption_;

    friend impl::WindowManager;
};

// ============================================================================
template <typename Derived_>
class Frame2 : public Frame
{
public:
    static boost::shared_ptr<Derived_> Create(
        ComponentId                     id,
        std::string const&              caption,
        uint32                          width,
        uint32                          height);

    Event<Derived_>& getEvent() { return m_event; }

    void invokeEvent();
    void setEvent(EventBase::Type const& type);

private:
    typedef Derived_                    Derived;
    typedef Derived                     Me;
    Event<Derived>                      m_event;
    std::vector<EventBase::Type>        m_eventQueue;
};

// ----------------------------------------------------------------------------
template <typename Derived_>
boost::shared_ptr<Derived_> Frame2<Derived_>::Create(
    ComponentId                     id,
    std::string const&              caption,
    uint32                          width,
    uint32                          height)
{
    boost::shared_ptr<Derived_> framePtr(new Derived_);
    framePtr->create(framePtr, id, caption, width, height);
    return framePtr;
}

// ----------------------------------------------------------------------------
template <typename Derived_>
void Frame2<Derived_>::invokeEvent()
{
    std::vector<EventBase::Type>::iterator it = m_eventQueue.begin();
    while (it != m_eventQueue.end())
    {
        m_event.invoke(*it++);
    }
    m_eventQueue.clear();
}

// ----------------------------------------------------------------------------
template <typename Derived_>
void Frame2<Derived_>::setEvent(EventBase::Type const& type)
{
    m_eventQueue.push_back(type);
}

END_NAMESPACE_NYANCO_GUI
