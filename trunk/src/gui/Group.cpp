/*!
    @file   Group.cpp
    @author dasyprocta
 */

#include "Group.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Group::Ptr Group::Create(
    ComponentId                         id,
    std::string const&                  label)
{
    Group* p = new Group;
    p->setMargin(Rect<sint32>(4, 18, 4, 4));
    p->m_label = label;
    return Ptr(p);
}

// ----------------------------------------------------------------------------
void Group::draw(
    Graphics&                           graphics)
{
    Rect<sint32> box = location_;
    box.setHeight(getHeight());
    box.top += 2; box.bottom -= 2;

    impl::FontInfo const& font = dynamic_cast<impl::Graphics&>(graphics).getFontInfo();
    sint32 const labelWidth  = font.charaWidth * m_label.size();
    sint32 const labelHeight = font.charaHeight;
    sint32 const labelHalfHeight = labelHeight / 2;

    graphics.setColor(0xff222222);
    // top
    graphics.drawLine(Point<sint32>(box.left, box.top+labelHalfHeight), Point<sint32>(box.left+4, box.top+labelHalfHeight));
    // left
    graphics.drawLine(Point<sint32>(box.left, box.top+labelHalfHeight), Point<sint32>(box.left, box.bottom-1));
    // label
    {
        Rect<sint32> clip = location_;
        graphics.drawText(Point<sint32>(box.left+8, box.top), m_label, 0xffeeeeee, clip);
    }
    // top2
    graphics.drawLine(Point<sint32>(box.left+12+labelWidth, box.top+labelHalfHeight), Point<sint32>(box.right, box.top+labelHalfHeight));

    graphics.setColor(0xff888888);
    // right
    graphics.drawLine(Point<sint32>(box.right, box.top+1+labelHalfHeight), Point<sint32>(box.right, box.bottom));
    // bottom
    graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));

    // top
    graphics.drawLine(Point<sint32>(box.left+2, box.top+2+labelHalfHeight), Point<sint32>(box.left+4, box.top+2+labelHalfHeight));
    // top2
    graphics.drawLine(Point<sint32>(box.left+12+labelWidth, box.top+2+labelHalfHeight), Point<sint32>(box.right-2, box.top+2+labelHalfHeight));
    // left
    graphics.drawLine(Point<sint32>(box.left+2, box.top+2+labelHalfHeight), Point<sint32>(box.left+2, box.bottom-1-2));

    graphics.setColor(0xff222222);
    // right
    graphics.drawLine(Point<sint32>(box.right-2, box.top+1+2+labelHalfHeight), Point<sint32>(box.right-2, box.bottom-2));
    // bottom
    graphics.drawLine(Point<sint32>(box.left+2, box.bottom-2), Point<sint32>(box.right-2, box.bottom-2));

    // Žq‚Ì•`‰æ
    using boost::bind;
    using boost::ref;
    std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
int Group::getHeight() const
{
    sint32 height = 0;
    foreach (ComponentPtr p, componentList_)
    {
        height += p->getHeight();
    }
    return height + margin_.top + margin_.bottom;
}

END_NAMESPACE_NYANCO_GUI
