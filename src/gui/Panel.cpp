/*!
    @file   Panel.cpp
    @author dasyprocta
 */

#include "Panel.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Panel::Ptr Panel::Create(ComponentId id)
{
    Panel* p = new Panel;
    p->setMargin(Rect<sint32>(0, 0, 0, 0));
    return Ptr(p);
}

// ----------------------------------------------------------------------------
void Panel::draw(Graphics& graphics)
{
    using boost::bind;
    using boost::ref;
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
int Panel::getHeight() const
{
    sint32 height = 0;
    foreach (Component::Ptr p, componentList_)
    {
        height += p->getHeight();
    }
    return height + margin_.top + margin_.bottom;
}

END_NAMESPACE_NYANCO_GUI
