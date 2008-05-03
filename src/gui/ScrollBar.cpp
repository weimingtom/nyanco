/*!
    @file   ScrollBar.cpp
    @author dasyprocta
 */

#include "ScrollBar.h"
#include "Graphics.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
ScrollBar::Ptr ScrollBar::Create(
    ComponentId                         id,
    Arg const&                          arg)
{
    ScrollBar* p = new ScrollBar;
    p->m_arg = arg;
    p->m_scrollOffset = 0;
    p->m_enableScroll = false;
    p->m_sliderDown = false;
    p->setLocation(Rect(0, 0, 10, arg.m_owner.get()->getClientWidth()));
    return ScrollBar::Ptr(p);
};

// ----------------------------------------------------------------------------
void ScrollBar::draw(Graphics& graphics)
{
    Rect rect = location_;
    graphics.drawFillRect(rect);

    // close button
    {
        Rect closeRegion = rect;
        closeRegion.bottom = closeRegion.top + 10;

        graphics.setRectColor(0xff444444);
        graphics.drawFillRect(closeRegion);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.right, closeRegion.top));
        graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.left, closeRegion.bottom-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(closeRegion.right, closeRegion.top+1), Point(closeRegion.right, closeRegion.bottom));
        graphics.drawLine(Point(closeRegion.left, closeRegion.bottom), Point(closeRegion.right, closeRegion.bottom));

        graphics.setColor(0xffeeeeee);
        sint32 half = (closeRegion.right - closeRegion.left) / 2;
        graphics.drawTriangle(Point(closeRegion.left+half, closeRegion.top+2), Point(closeRegion.right-1, closeRegion.bottom-1), Point(closeRegion.left+2, closeRegion.bottom-1));
    }

    // close button
    {
        Rect closeRegion = rect;
        closeRegion.bottom -= 1;
        closeRegion.top = closeRegion.bottom - 10;

        graphics.setRectColor(0xff444444);
        graphics.drawFillRect(closeRegion);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.right, closeRegion.top));
        graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.left, closeRegion.bottom-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(closeRegion.right, closeRegion.top+1), Point(closeRegion.right, closeRegion.bottom));
        graphics.drawLine(Point(closeRegion.left, closeRegion.bottom), Point(closeRegion.right, closeRegion.bottom));

        graphics.setColor(0xffeeeeee);
        sint32 half = (closeRegion.right - closeRegion.left) / 2;
        graphics.drawTriangle(Point(closeRegion.left+half, closeRegion.bottom-1), Point(closeRegion.left+2, closeRegion.top+2), Point(closeRegion.right-1, closeRegion.top+2));
    }

    // scroll
    if (m_enableScroll)
    {
        int clientHeight = m_arg.m_owner->getClientWidth();
        int contentHeight = m_arg.m_owner->getContentWidth();
        int barLength = rect.getHeight() - 20;

        float32 per = static_cast<float32>(clientHeight) / contentHeight;
        m_sliderSize = barLength * per;
        sint32 sliderLength = static_cast<sint32>(m_sliderSize);
        float32 scrollSize = m_arg.m_owner->getOneContentSize() * (static_cast<float32>(barLength) / contentHeight);
        m_scrollSize = scrollSize;
        m_arg.m_owner->m_scrollClientOffset = static_cast<sint32>((m_scrollOffset / scrollSize) * m_arg.m_owner->getOneContentSize());

        Rect slider = rect;
        slider.top += 11;
        slider.top += m_scrollOffset;
        slider.bottom = slider.top + sliderLength-2;
        graphics.setRectColor(0xff444444);
        graphics.drawFillRect(slider);

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(slider.left, slider.top), Point(slider.right, slider.top));
        graphics.drawLine(Point(slider.left, slider.top), Point(slider.left, slider.bottom-1));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(slider.right, slider.top+1), Point(slider.right, slider.bottom));
        graphics.drawLine(Point(slider.left, slider.bottom), Point(slider.right, slider.bottom));

        graphics.setColor(0xff222222);
        graphics.drawLine(Point(slider.left+2, slider.top+2), Point(slider.right-2, slider.top+2));
        graphics.drawLine(Point(slider.left+2, slider.top+2), Point(slider.left+2, slider.bottom-1-2));

        graphics.setColor(0xff888888);
        graphics.drawLine(Point(slider.right-2, slider.top+1+2), Point(slider.right-2, slider.bottom-2));
        graphics.drawLine(Point(slider.left+2, slider.bottom-2), Point(slider.right-2, slider.bottom-2));
    }
}

// ----------------------------------------------------------------------------
void ScrollBar::update()
{
    int clientHeight = m_arg.m_owner->getClientWidth();
    int numContents  = m_arg.m_owner->getNumContents();
    int contentSize  = m_arg.m_owner->getOneContentSize();
    int contentHeight = numContents * contentSize;

    m_enableScroll = (contentHeight > clientHeight);
    if (m_enableScroll)
    {
        
    }
}

// ----------------------------------------------------------------------------
bool ScrollBar::onMouseProcess(MouseCommand const& mouse)
{
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
                if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight() - 20)
                    m_scrollOffset = location_.getHeight() - 20 - m_sliderSize;
                return true;
            }

            // slider
            int clientHeight = m_arg.m_owner->getClientWidth();
            int contentHeight = m_arg.m_owner->getContentWidth();
            int barLength = location_.getHeight() - 20;

            float32 per = static_cast<float32>(clientHeight) / contentHeight;
            sint32 sliderLength = static_cast<sint32>(barLength * per);

            Rect slider = location_;
            slider.top += 10;
            slider.bottom -= 11;
            slider.top += m_scrollOffset;
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
            if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight() - 20)
                m_scrollOffset = location_.getHeight() - 20 - m_sliderSize;

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
            int clientHeight = m_arg.m_owner->getClientWidth();
            int contentHeight = m_arg.m_owner->getContentWidth();
            int barLength = location_.getHeight() - 20;

            float32 per = static_cast<float32>(clientHeight) / contentHeight;
            sint32 sliderLength = static_cast<sint32>(barLength * per);

            Rect slider = location_;
            slider.top += 10;
            slider.bottom -= 11;
            slider.top += m_scrollOffset;
            slider.bottom = slider.top + sliderLength;

            if (mouse.posY < location_.top+10)
            {
                m_scrollOffset = 0.f;
            }
            else if (mouse.posY >= location_.bottom-11)
            {
                m_scrollOffset = location_.getHeight() - 20 - m_sliderSize;
            }
            else
            {
                float32 move = static_cast<float32>(contentHeight) / barLength;
                m_scrollOffset += mouse.moveY;
                if (m_scrollOffset < 0) m_scrollOffset = 0;
                if (static_cast<sint32>(m_scrollOffset + m_sliderSize) > location_.getHeight() - 20)
                    m_scrollOffset = location_.getHeight() - 20 - m_sliderSize;
            }
        }
        return true;
    }
    return false;
}

END_NAMESPACE_NYANCO_GUI
