/*!
    @file   Component.cpp
    @author dasyprocta
 */

#include "Component.h"
#include "Container.h"
#include "Event.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
sint32 Component::relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY)
{
    location_.right  = parentLeft + parentWidth;
    location_.left   = parentLeft;
    location_.bottom = locationY + getHeight();
    location_.top    = locationY;
    return getHeight() + locationY;
}

// ----------------------------------------------------------------------------
Component::Ptr Component::checkHit(int x, int y)
{
    return isPointInner(Point(x, y))? shared_from_this(): ComponentPtr();
}

// ----------------------------------------------------------------------------
Component::Ptr Component::searchById(sint32 id)
{
    if (m_id == id)
    {
        return shared_from_this();
    }
    return Component::Ptr();
}

// ----------------------------------------------------------------------------
bool Component::isPointInner(Point const& point)
{
    if (location_.left <= point.x && point.x <= location_.right &&
        location_.top  <= point.y && point.y <= location_.bottom)
        return true;
    return false;
}

// ----------------------------------------------------------------------------
void Component::focus()
{
    focused_ = true;
}

// ----------------------------------------------------------------------------
void Component::defocus()
{
    focused_ = false;
}

// ----------------------------------------------------------------------------
bool Component::isFocused() const
{
    return focused_;
}

// ----------------------------------------------------------------------------
void Component::setLocation(Rect const& location)
{
    location_ = location;
}

// ----------------------------------------------------------------------------
void Component::setX(int x)
{
    location_.right = x + location_.getWidth();
    location_.left  = x;
}

// ----------------------------------------------------------------------------
void Component::setY(int y)
{
    location_.bottom = y + location_.getHeight();
    location_.top    = y;
}

// ----------------------------------------------------------------------------
void Component::setWidth(int width)
{
    location_.right = location_.left + width;
}

// ----------------------------------------------------------------------------
void Component::setHeight(int height)
{
    location_.bottom = location_.top + height;
}

// ----------------------------------------------------------------------------
int Component::getHeight() const
{
    return location_.bottom - location_.top;
}

// ----------------------------------------------------------------------------
int Component::getWidth() const
{
    return location_.right - location_.left;
}

// ----------------------------------------------------------------------------
void Component::update()
{
    
}

// ----------------------------------------------------------------------------
void Component::move(int x, int y)
{
    location_.left      += x;
    location_.right     += x;
    location_.top       += y;
    location_.bottom    += y;
}

// ----------------------------------------------------------------------------
void Component::attachParent(ComponentPtr parent)
{
    parent_ = parent;
}

// ----------------------------------------------------------------------------
void Component::detachParent()
{
    parent_ = ComponentPtr();
}

// ----------------------------------------------------------------------------
ComponentPtr Component::getTopLevelContainer() const
{
    if (parent_.get() == 0) return ComponentPtr();

    ComponentPtr p = parent_;
    while (p->parent_.get() != 0)
    {
        p = p->parent_;
    }
    return p;
}

// ----------------------------------------------------------------------------
EventServer* Component::getEventServer() const
{
    Component::Ptr ptr = getTopLevelContainer();
    return dynamic_cast<EventServer*>(ptr.get());
}

END_NAMESPACE_NYANCO_GUI
