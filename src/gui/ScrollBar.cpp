/*!
    @file   ScrollBar.cpp
    @author dasyprocta
 */

#include "ScrollBar.h"
#include "Graphics.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
void ScrollBar::Owner::createScrollBar(
    Owner::Ptr                          owner,
    Arg*                                verticalArg,
    Arg*                                horizontalArg)
{
    if (verticalArg != 0)
    {
        m_scrollBar[0] = ScrollBar::Create((*verticalArg).type(Vertical), owner);
    }
    if (horizontalArg != 0)
    {
        m_scrollBar[1] = ScrollBar::Create((*horizontalArg).type(Horizontal), owner);
    }
}

// ----------------------------------------------------------------------------
void ScrollBar::Owner::updateScrollBar()
{
    m_scrollBar[0]->update();
    m_scrollBar[1]->update();
}

// ----------------------------------------------------------------------------
void ScrollBar::Owner::drawScrollBar(Graphics& g)
{
    if (m_scrollBar[0]->isVisible()) m_scrollBar[0]->draw(g);
    if (m_scrollBar[1]->isVisible()) m_scrollBar[1]->draw(g);
}

// ----------------------------------------------------------------------------
bool ScrollBar::Owner::onScrollBarMouseProcess(MouseCommand const& mouse)
{
    if (m_scrollBar[0]->isVisible())
    {
        if (m_scrollBar[0]->onMouseProcess(mouse))
            return true;
    }
    if (m_scrollBar[1]->isVisible())
        return m_scrollBar[1]->onMouseProcess(mouse);
    return false;
}

// ----------------------------------------------------------------------------
void ScrollBar::Owner::getClientSizeWithScrollBar(Size& size) const
{
    Rect client;
    getScrolledClientRect(client);
    size.width  = client.getWidth() - (m_scrollBar[0]->isVisible()? m_scrollBar[0]->getWidth(): 0);
    size.height = client.getHeight() - (m_scrollBar[1]->isVisible()? m_scrollBar[1]->getHeight(): 0);
}

// ----------------------------------------------------------------------------
ScrollBar::Ptr ScrollBar::Create(
    Arg const&                          arg,
    Owner::Ptr                          owner,
    ComponentId                         id)
{
    ScrollBar* p = new ScrollBar;
    p->m_arg = arg;
    p->m_scrollOffset = 0;
    p->m_enableScroll = false;
    p->m_sliderDown = false;

    if (p->m_arg.m_type == Vertical)
    {
        p->setLocation(Rect(0, 0, 10, 0));
    }
    else if (p->m_arg.m_type == Horizontal)
    {
        p->setLocation(Rect(0, 0, 0, 10));
    }
    else assert(0);

    p->m_owner = owner;

    return ScrollBar::Ptr(p);
};

// ----------------------------------------------------------------------------
void ScrollBar::draw(Graphics& graphics)
{
    ComponentGraphics g(graphics);

    Rect rect = location_;
    graphics.drawFillRect(Rect(rect.left, rect.top, rect.right, rect.bottom-1));

    Rect button[2];
    calcButtonRect(button[0], button[1]);

    // close button
    {
        g.drawFrame(button[0]);
        graphics.setColor(0xffeeeeee);
        sint32 half = (button[0].right - button[0].left - 2) / 2;
//        graphics.drawTriangle(Point(button[0].left+half, button[0].top+2), Point(button[0].right-1, button[0].bottom-1), Point(button[0].left+2, button[0].bottom-1));
    }

    // close button
    {
        g.drawFrame(button[1]);
        graphics.setColor(0xffeeeeee);
        sint32 half = (button[1].right - button[1].left) / 2;
//        graphics.drawTriangle(Point(button[1].left+half, button[1].bottom-1), Point(button[1].left+2, button[1].top+2), Point(button[1].right-1, button[1].top+2));
    }

    // scroll
    if (m_enableScroll)
    {
        Rect slider;
        calcSliderRect(slider);
        g.drawFrame(slider);
        Rect inner = slider;
        inner.extend(-2);
        g.drawFrame(inner, false);
    }
}

// ----------------------------------------------------------------------------
void ScrollBar::update()
{
    Owner::Ptr owner = m_owner.lock();
    if (!owner) return;

    Rect client;
    owner->getScrolledClientRect(client);

    float32 const contentSize       = getContentSize();
    float32 const clientSize        = getClientSize();
    sint32  const unitInclementSize = getUnitInclementSize();

    sint32 barLength;
    if (m_arg.m_type == Vertical)
    {
        setX(client.right - getWidth());
        setY(client.top);
        setHeight(clientSize);
        barLength = client.getHeight() - 20 - (owner->getScrollBar(Horizontal)->isVisible()? 10: 0);
    }
    else if (m_arg.m_type == Horizontal)
    {
        setX(client.left);
        setY(client.bottom - getHeight());
        setWidth(clientSize);
        barLength = client.getWidth() - 20 - (owner->getScrollBar(Vertical)->isVisible()? 10: 0);
    }
    else assert(0);

    float32 per      = clientSize / contentSize;
    m_sliderSize     = barLength * per;

    float32 scrollSize = unitInclementSize * (static_cast<float32>(barLength) / contentSize);
    m_scrollSize = scrollSize;

    m_scrollClientOffset = static_cast<sint32>((m_scrollOffset / scrollSize) * unitInclementSize);
    if (m_scrollOffset < 0)
    {
        m_scrollClientOffset = 0;
        m_scrollOffset       = 0;
    }
    else if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > barLength)
    {
        m_scrollClientOffset = contentSize - clientSize;
        m_scrollOffset       = barLength-m_sliderSize;
    }

    m_enableScroll = (contentSize > clientSize);
}

// ----------------------------------------------------------------------------
bool ScrollBar::onMouseProcess(MouseCommand const& mouse)
{
    Owner::Ptr owner = m_owner.lock();
    if (!owner) return false;

    Rect client;
    owner->getScrolledClientRect(client);

    if (mouse.onPushLeft)
    {
        if (isPointInner(Point(mouse.posX, mouse.posY)))
        {
            // up button
            Rect closeRegion = location_;
            closeRegion.bottom = closeRegion.top + 10;
            if (closeRegion.isInnerPoint(mouse.posX, mouse.posY))
            {
                m_scrollOffset -= m_scrollSize;
                if (m_scrollOffset < 0) m_scrollOffset = 0;
                return true;
            }
            // down button
            closeRegion = location_;
            closeRegion.bottom -= 1;
            closeRegion.top = closeRegion.bottom - 10;
            if (closeRegion.isInnerPoint(mouse.posX, mouse.posY))
            {
                m_scrollOffset += m_scrollSize;
                if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight()-20)
                    m_scrollOffset = location_.getHeight()-20-m_sliderSize;
                return true;
            }

            // slider
            int clientHeight = getClientSize();
            int contentHeight = getContentSize();
            int barLength = location_.getHeight() - 20;

            float32 per = static_cast<float32>(clientHeight) / contentHeight;
            sint32 sliderLength = static_cast<sint32>(barLength * per);

            Rect slider = location_;
            slider.top += 10;
            slider.bottom -= 10;
            slider.top += static_cast<sint32>(m_scrollOffset);
            slider.bottom = slider.top + sliderLength;
            if (slider.isInnerPoint(mouse.posX, mouse.posY))
            {
                m_sliderDown = true;
                return true;
            }

            // bar スライダー外の領域
            if (mouse.posY < slider.top)
            {
                m_scrollOffset -= sliderLength;
            }
            else if (mouse.posY > slider.bottom)
            {
                m_scrollOffset += sliderLength;
            }
            if (m_scrollOffset < 0) m_scrollOffset = 0;
            if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight()-20)
                m_scrollOffset = location_.getHeight()-20-m_sliderSize;

            return true;
        }
    }
    else if (mouse.onUpLeft)
    {
        m_sliderDown = false;
    }
    else if (mouse.onDownLeft)
    {
        if (m_sliderDown)
        {
            int clientHeight = getClientSize();
            int contentHeight = getContentSize();
            int barLength = location_.getHeight()-20;

            float32 per = static_cast<float32>(clientHeight) / contentHeight;
            sint32 sliderLength = static_cast<sint32>(barLength * per);

            Rect slider = client;
            slider.top += 10;
            slider.bottom -= 10;
            slider.top += m_scrollOffset;
            slider.bottom = slider.top + sliderLength;

            if (mouse.posY < location_.top+10)
            {
                m_scrollOffset = 0.f;
            }
            else if (mouse.posY >= location_.bottom-10)
            {
                m_scrollOffset = location_.getHeight()-20-m_sliderSize;
            }
            else
            {
                float32 move = static_cast<float32>(contentHeight) / barLength;
                m_scrollOffset += mouse.moveY;
                if (m_scrollOffset < 0) m_scrollOffset = 0;
                if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight()-20)
                    m_scrollOffset = location_.getHeight()-20-m_sliderSize;
            }
        }
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
sint32 ScrollBar::getContentOffset() const
{
    return m_scrollClientOffset;
}

// ----------------------------------------------------------------------------
bool ScrollBar::isScrollEnabled() const
{
    return m_enableScroll;
}

// ----------------------------------------------------------------------------
bool ScrollBar::isVisible() const
{
    return m_enableScroll || (!m_arg.m_disable);
}

// ----------------------------------------------------------------------------
sint32 ScrollBar::getClientSize() const
{
    Owner::Ptr owner = m_owner.lock();

    Rect clientRect;
    owner->getScrolledClientRect(clientRect);

    sint32 size;
    if (m_arg.m_type == Vertical)
    {
        size = clientRect.getHeight() - (owner->getScrollBar(Horizontal)->isVisible()? 10: 0);
    }
    else if (m_arg.m_type == Horizontal)
    {
        size = clientRect.getWidth() - (owner->getScrollBar(Vertical)->isVisible()? 10: 0);
    }
    else assert(0);

    return size;
}

// ----------------------------------------------------------------------------
sint32 ScrollBar::getContentSize() const
{
    Owner::Ptr owner = m_owner.lock();

    Size contentSize;
    owner->getScrolledContentSize(contentSize);

    sint32 size;
    if (m_arg.m_type == Vertical)
    {
        size = contentSize.height;
    }
    else if (m_arg.m_type == Horizontal)
    {
        size = contentSize.width;
    }
    else assert(0);

    return size;
}

// ----------------------------------------------------------------------------
sint32 ScrollBar::getUnitInclementSize() const
{
    Owner::Ptr owner = m_owner.lock();

    Size inclementSize;
    owner->getScrolledUnitInclementSize(inclementSize);

    sint32 size;
    if (m_arg.m_type == Vertical)
    {
        size = inclementSize.height;
    }
    else if (m_arg.m_type == Horizontal)
    {
        size = inclementSize.width;
    }
    else assert(0);

    return size;
}

// ----------------------------------------------------------------------------
void ScrollBar::calcButtonRect(Rect& b1, Rect& b2) const
{
    b1 = b2 = location_;
    if (m_arg.m_type == Vertical)
    {
        b1.bottom = b1.top + 10;
        b2.top    = b2.bottom - 10;
    }
    else if (m_arg.m_type == Horizontal)
    {
        b1.right = b1.left + 10;
        b2.left  = b2.right - 10;
    }
    else assert(0);
}

// ----------------------------------------------------------------------------
void ScrollBar::calcSliderRect(Rect& slider) const
{
    slider = location_;
    if (m_arg.m_type == Vertical)
    {
        slider.top += 10;
        slider.top += m_scrollOffset;
        slider.bottom = slider.top + static_cast<sint32>(m_sliderSize)+1;
    }
    else if (m_arg.m_type == Horizontal)
    {
        slider.left += 10;
        slider.left += m_scrollOffset;
        slider.right = slider.left + static_cast<sint32>(m_sliderSize);
    }
    else assert(0);
}

END_NAMESPACE_NYANCO_GUI
