/*!
    @file   Rollup.cpp
    @author dasyprocta
 */

#include "Rollup.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Rollup::Ptr Rollup::Create(
    Arg<> const&                        arg,
    ComponentId                         id)
{
    Ptr p(new Rollup);
    p->setMargin(Rect<sint32>(4, 18, 4, 4));
    p->m_arg = arg;
    return p;
}

// ----------------------------------------------------------------------------
void Rollup::draw(Graphics& graphics)
{
    Rect<sint32> box = location_;
    box.left    += 6;
    box.right   -= 6;
    box.top     += 2;
    box.bottom   = box.top + 14;

    //graphics.setColor(0xff444444);
    if (!isFocused())
        graphics.setRectColor(0xff777777, 0xff777777, 0xff333333, 0xff333333);
    else
        graphics.setRectColor(0xffaaaaaa, 0xffaaaaaa, 0xff666666, 0xff666666);
    graphics.drawFillRect(box);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.right, box.top));
    graphics.drawLine(Point<sint32>(box.left, box.top), Point<sint32>(box.left, box.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point<sint32>(box.right, box.top+1), Point<sint32>(box.right, box.bottom));
    graphics.drawLine(Point<sint32>(box.left, box.bottom), Point<sint32>(box.right, box.bottom));

    Rect<sint32> clip = location_;
    clip.left += 2; clip.right -= 2;
    size_t textWidth = m_arg.m_label.size() * 6;
    size_t left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point<sint32>(box.left + 2, box.top + 2), m_arg.m_label, 0xffeeeeee, clip);

    impl::FontInfo const& font = dynamic_cast<impl::Graphics&>(graphics).getFontInfo();
    sint32 const labelWidth  = font.charaWidth * m_arg.m_label.size();
    sint32 const labelHeight = font.charaHeight;
    sint32 const labelHalfHeight = labelHeight / 2;

    // top
    graphics.setColor(0xff888888);
    graphics.drawLine(Point<sint32>(box.left-4, box.top+2+labelHalfHeight), Point<sint32>(box.left, box.top+2+labelHalfHeight));
    graphics.drawLine(Point<sint32>(box.right, box.top+2+labelHalfHeight), Point<sint32>(box.right+4, box.top+2+labelHalfHeight));
    graphics.setColor(0xff222222);
    graphics.drawLine(Point<sint32>(box.left-6, box.top+labelHalfHeight), Point<sint32>(box.left, box.top+labelHalfHeight));
    graphics.drawLine(Point<sint32>(box.right, box.top+labelHalfHeight), Point<sint32>(box.right+6, box.top+labelHalfHeight));

    Rect<sint32> border = location_;
    border.top+=(2+labelHalfHeight); border.bottom-=2;
    if (!m_collapsed)
    {
        // 子の描画
        using boost::bind;
        using boost::ref;
        std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
    }
    graphics.setColor(0xff888888);
    // left
    graphics.drawLine(Point<sint32>(border.left+2, border.top+2), Point<sint32>(border.left+2, location_.bottom-1-2));
    // right
    graphics.drawLine(Point<sint32>(border.right, border.top+1), Point<sint32>(border.right, border.bottom));
    // bottom
    graphics.drawLine(Point<sint32>(border.left, border.bottom), Point<sint32>(border.right, border.bottom));

    graphics.setColor(0xff222222);
    // left
    graphics.drawLine(Point<sint32>(border.left, border.top), Point<sint32>(border.left, location_.bottom-1));
    // right
    graphics.drawLine(Point<sint32>(border.right-2, border.top+1+2), Point<sint32>(border.right-2, border.bottom-2));
    // bottom
    graphics.drawLine(Point<sint32>(border.left+2, border.bottom-2), Point<sint32>(border.right, border.bottom-2));
}

// ----------------------------------------------------------------------------
int Rollup::getHeight() const
{
    if (!m_collapsed)
    {
        sint32 height = 0;
        foreach (Component::Ptr p, componentList_)
        {
            height += p->getHeight();
        }
        return height + margin_.top + margin_.bottom;
    }
    return margin_.top + margin_.bottom;
}

// ----------------------------------------------------------------------------
bool Rollup::onMouseProcess(MouseCommand const& command)
{
    // TODO: ボタン境界チェック
    if (command.onPushLeft)
    {
        m_collapsed = !m_collapsed;
        location_.bottom = location_.top + getHeight();
    }
    return false;
}

// ----------------------------------------------------------------------------
bool Rollup::onKeyboardProcess(KeyboardCommand const& command)
{
    if (command.code == KeyCode::Return)
    {
        m_collapsed = !m_collapsed;
        location_.bottom = location_.top + getHeight();
    }
    return false;
}

// ----------------------------------------------------------------------------
Component::Ptr Rollup::getFirstComponent() const
{
    if (m_collapsed)
    {
        return Component::Ptr();
    }
    return Container::getFirstComponent();
}

// ----------------------------------------------------------------------------
Component::Ptr Rollup::getNextComponent(Component::ConstPtr component) const
{
    if (m_collapsed)
    {
        return (boost::dynamic_pointer_cast<ComponentIterator>(getParent()))->getNextComponent(component);
    }
    return Container::getNextComponent(component);
}

// TODO: Container::checkHit のオーバーライド
// 折り畳み時に子コンポーネントのヒットチェックをしないようにする

END_NAMESPACE_NYANCO_GUI
