/*!
    @file   Container.cpp
    @author dasyprocta
 */

#include "Container.h"
#include <algorithm>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

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
    componentPtr->attachParent(ComponentPtr(this));
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
void Container::detach(
    std::string const&              componentName)
{

}

// ----------------------------------------------------------------------------
void Container::resize(int parentWidth)
{
    int const childWidth = parentWidth - (margin_.left + margin_.right);
    foreach (ComponentPtr comp, componentList_)
    {
        comp->resize(childWidth);
    }
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



END_NAMESPACE_NYANCO_GUI
