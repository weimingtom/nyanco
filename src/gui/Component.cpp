/*!
    @file   Component.cpp
    @author dasyprocta
 */

#include "Component.h"
#include "Container.h"

namespace nyanco { namespace gui
{

class ComponentImpl
{

};

    void Component::resize(int parentWidth)
    {
        location_.right = location_.left + parentWidth;
    }

    ComponentPtr Component::checkHit(int x, int y)
    {
        return isPointInner(Point(x, y))? shared_from_this(): ComponentPtr();
    }

    bool Component::isPointInner(Point const& point)
    {
        if (location_.left <= point.x && point.x <= location_.right &&
            location_.top  <= point.y && point.y <= location_.bottom)
            return true;
        return false;
    }

    void Component::focus()
    {
        focused_ = true;
    }

    bool Component::isFocused() const
    {
        return focused_;
    }

    void Component::setName(std::string const& name)
    {
        name_ = name;
    }

    void Component::setLocation(Rect const& location)
    {
        location_ = location;
    }

    void Component::setX(int x)
    {
        location_.right = x + location_.getWidth();
        location_.left  = x;
    }

    void Component::setY(int y)
    {
        location_.bottom = y + location_.getHeight();
        location_.top    = y;
    }

    void Component::setWidth(int width)
    {
        location_.right = location_.left + width;
    }

    int Component::getHeight() const
    {
        return location_.bottom - location_.top;
    }

    void Component::update()
    {
        
    }

    void Component::move(int x, int y)
    {
        location_.left      += x;
        location_.right     += x;
        location_.top       += y;
        location_.bottom    += y;
    }

} } // namespace nyanco::gui
