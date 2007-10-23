/*!
    @file   Container.cpp
    @author dasyprocta
 */

#include "Container.h"
#include <algorithm>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

namespace nyanco { namespace gui
{

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
        componentList_.push_back(componentPtr);
    }

    void Container::detach(
        ComponentPtr                    componentPtr)
    {
        componentList_.remove(componentPtr);

#if 0
        ComponentList::iterator it = std::find(
            componentList_.begin(), componentList_.end(), componentPtr);
        if (it != componentList_.end())
        {

        }
#endif
    }

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

} } // namespace nyanco::gui
