/*!
    @file   Container.cpp
    @author dasyprocta
 */

#include "Container.h"
#include <algorithm>

namespace nyanco { namespace gui
{

    void Container::attach(
        Component*                      componentPtr)
    {
        componentList_.push_back(componentPtr);
    }

    void Container::detach(
        Component*                      componentPtr)
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

} } // namespace nyanco::gui
