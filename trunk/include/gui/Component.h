#pragma once

/*!
    @file   Component.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Event.h"
#include <string>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class Component;
class Graphics;
class Container;
class ContextMenu;
class EventServer;

namespace impl { class WindowManager; }

typedef boost::shared_ptr<Component> ComponentPtr;

class Component
    : public EventClient,
      public boost::enable_shared_from_this<Component>
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Component);

    ComponentId getId() const { return m_id; }

    virtual void focus();
    virtual void defocus();
    virtual bool isFocused() const;
    virtual bool enableFocus() const { return true; }

    virtual bool isEnabled() const { return m_enabled; }
    void enable(bool flag) { m_enabled = flag; }

    virtual int getHeight() const;
    virtual int getWidth() const;

    Rect<sint32>& getLocation() { return location_; }
    Rect<sint32> const& getLocation() const { return location_; }
    virtual void setLocation(Rect<sint32> const& location);
    //void setPosition(int x, int y);
    virtual void setX(int x);
    virtual void setY(int y);
    virtual void setWidth(int width);
    virtual void setHeight(int height);

    virtual void draw(Graphics& graphics) = 0;
    virtual sint32 relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY);

    //virtual void resize(int parentWidth);
    virtual Component::Ptr searchById(sint32 id);
    virtual Component::Ptr checkHit(int x, int y);
    virtual void update();
    virtual void move(int x, int y);

    virtual bool onMouseProcess(MouseCommand const& mouse) { return false; }
    virtual bool onKeyboardProcess(KeyboardCommand const& keyboard) { return false;}

    bool isPointInner(Point<sint32> const& point);

    void attachParent(ComponentPtr parent);
    void detachParent();

    Component::Ptr getParent() const { return parent_; }

protected:
    void setId(ComponentId id) { m_id = id; }
    EventServer* getEventServer() const;
    ComponentPtr getTopLevelContainer() const;

protected:
    ComponentId                     m_id;
    Rect<sint32>                            location_;
    bool                            focused_;
    bool                            m_enabled;
    ComponentPtr                    parent_;

    Component() : m_id(NonspecificId), m_enabled(true), focused_(false) {}

    friend ContextMenu;
    friend Container;
    friend impl::WindowManager;
};

class ComponentIterator
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(ComponentIterator);

    virtual Component::Ptr getFirstComponent() const = 0;
    virtual Component::Ptr getLastComponent() const = 0;
    virtual Component::Ptr getNextComponent(Component::ConstPtr component) const = 0;
    virtual Component::Ptr getPrevComponent(Component::ConstPtr component) const = 0;
};

class MultiComponent : public Component, public ComponentIterator
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(MultiComponent);
};

END_NAMESPACE_NYANCO_GUI
