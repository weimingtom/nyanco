/*!
    @file   Container.cpp
    @author dasyprocta
 */

#include "Container.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define foreach BOOST_FOREACH

using boost::bind;
using boost::ref;

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
void Container::attach(
    ComponentPtr                    componentPtr)
{
    {
        Rect childLocation;
        childLocation.left   = location_.left + margin_.left;
        childLocation.right  = location_.right - margin_.right;
        componentPtr->setX(childLocation.left);
        componentPtr->setWidth(childLocation.right - childLocation.left);
    }
    componentPtr->attachParent(Component::Ptr(this));
    componentList_.push_back(componentPtr);
}

// ----------------------------------------------------------------------------
void Container::detach(
    ComponentPtr                    componentPtr)
{
    componentPtr->detachParent();
    componentList_.remove(componentPtr);
}

// ----------------------------------------------------------------------------
sint32 Container::relocate(sint32 left, sint32 width, sint32 locationY)
{
    locationY += margin_.top;
    foreach (ComponentPtr p, componentList_)
    {
        p->setX(location_.left + margin_.left);
        p->setY(locationY);
        p->resize(location_.getWidth() - margin_.left * 2);
        int currentY = p->relocate(location_.left + margin_.left, location_.getWidth() - margin_.left * 2, locationY + margin_.top);
        locationY = currentY;
    }
    location_.bottom = locationY;
    return locationY;
}

// ----------------------------------------------------------------------------
void Container::resize(int parentWidth)
{
    Component::resize(parentWidth);
    int const childWidth = parentWidth - (margin_.left + margin_.right);
    foreach (ComponentPtr comp, componentList_)
    {
        comp->resize(childWidth);
    }
}

// ----------------------------------------------------------------------------
void Container::move(int x, int y)
{
    Component::move(x, y);
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::move, _1, x, y));
}

// ----------------------------------------------------------------------------
void Container::setMargin(Rect const& margin)
{
    margin_ = margin;
}

// ----------------------------------------------------------------------------
ComponentPtr Container::checkHit(int x, int y)
{
    if (isPointInner(Point(x, y)))
    {
        foreach (ComponentPtr comp, componentList_)
        {
            ComponentPtr hit = comp->checkHit(x, y);
            if (hit != 0) return hit;
        }
        return shared_from_this();
    }
    return ComponentPtr();
}


// ----------------------------------------------------------------------------


END_NAMESPACE_NYANCO_GUI
