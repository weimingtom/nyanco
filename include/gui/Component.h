#pragma once

/*!
    @file   Component.h
    @author dasyprocta
 */

#include "gui_base.h"
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

class Component : public boost::enable_shared_from_this<Component>
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Component);

    ComponentId getId() const { return m_id; }

    virtual void focus();
    virtual void defocus();
    virtual bool isFocused() const;
    virtual bool enableFocus() const { return true; }

    virtual int getHeight() const;
    virtual int getWidth() const;

    virtual void setLocation(Rect const& location);
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

    bool isPointInner(Point const& point);

    void attachParent(ComponentPtr parent);
    void detachParent();

protected:
    EventServer* getEventServer() const;
    ComponentPtr getTopLevelContainer() const;

protected:
    ComponentId                     m_id;
    Rect                            location_;
    bool                            focused_;
    ComponentPtr                    parent_;

    Component() : focused_(false) {}

    friend ContextMenu;
    friend Container;
    friend impl::WindowManager;
};

END_NAMESPACE_NYANCO_GUI
