#pragma once

/*!
    @file   Window.h
    @author dasyprocta
 */

#include "Container.h"
#include "Event.h"
#include "Dock.h"
#include <boost/iterator_adaptors.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class Window
    : public EventServer,
      public boost::enable_shared_from_this<Window>
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Window);

    virtual void moveTo(int x, int y);
    virtual void move(int x, int y);

    virtual void draw(Graphics& graphics) = 0;
    virtual void update() {}

    virtual void focus(Component::Ptr component);
    virtual void defocus();
    virtual void revertFocus();

    virtual Component::Ptr getFocusedComponent() const;
    virtual Component::Ptr getHitComponent(int x, int y) { return Component::Ptr(); }

    WindowId getId() const          { return m_id; }

    bool isPointInner(Point const& point) const;

    struct iterator : boost::iterator_adaptor<
        iterator,
        std::list<Component::Ptr>::iterator,
        Component::Ptr,
        boost::bidirectional_traversal_tag>
    {
        iterator(std::list<Component::Ptr>::iterator it)
            : boost::iterator_adaptor<
                iterator,
                std::list<Component::Ptr>::iterator,
                Component::Ptr,
                boost::bidirectional_traversal_tag>(it) {}

        void increment()
        {

        }

        void decrement()
        {

        }

        Component::Ptr dereference() const
        {

        }
    };

protected:
    void setId(WindowId id)         { m_id = id; }
    Rect& getLocation()             { return m_location; }
    Rect const& getLocation() const { return m_location; }

private:
    //! リターンキーを優先的にキャプチャするコンポーネント
    Component::WeakPtr                  m_returnComponent;
    Component::WeakPtr                  m_focusedComponent;
    Component::WeakPtr                  m_prevFocused;

    WindowId                            m_id;
    Rect                                m_location;
};

class DockableWindow
    : public Window, public Dockable
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(DockableWindow);
};

END_NAMESPACE_NYANCO_GUI
