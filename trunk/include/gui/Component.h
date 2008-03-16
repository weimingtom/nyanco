#pragma once

/*!
    @file   Component.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class Component;
class Graphics;
class Container;
class Frame;
class ContextMenu;

namespace impl { class WindowManager; }

typedef boost::shared_ptr<Component> ComponentPtr;

class Component : public boost::enable_shared_from_this<Component>
{
public:
    std::string const& getName() const { return name_; }
    void focus();
    bool isFocused() const;

    virtual int getHeight() const;

    void setName(std::string const& name);
    void setLocation(Rect const& location);
    //void setPosition(int x, int y);
    void setX(int x);
    void setY(int y);
    void setWidth(int width);

    virtual void draw(Graphics& graphics) = 0;
    virtual void resize(int parentWidth);
    virtual boost::shared_ptr<Component> checkHit(int x, int y);
    virtual void update();
    virtual void move(int x, int y);

    virtual void onMouseProcess(MouseCommand const& mouse) {}
    virtual void onKeyboardPtrocess(KeyboardCommand const& keyboard) {}

    bool isPointInner(Point const& point);

protected:
    void attachParent(ComponentPtr parent);
    void detachParent();
    ComponentPtr getTopLevelContainer();

protected:
    std::string                     name_;
    Rect                            location_;
    bool                            focused_;
    ComponentPtr                    parent_;

    Component() : focused_(false) {}

    friend ContextMenu;
    friend Container;
    friend impl::WindowManager;
};

END_NAMESPACE_NYANCO_GUI
