#pragma once

/*!
    @file   SplitPanel.h
    @author dasyprocta
 */

#include "Component.h"
#include "Panel.h"
#include <algorithm>
#include <boost/array.hpp>
#include <boost/bind.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

template <int Num_>
class SplitPanel : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(typename SplitPanel<Num_>);

    static typename SplitPanel<Num_>::Ptr Create(
        ComponentId                     id);

    template <int Index_>
    Panel::Ptr get() { return m_panels[Index_]; }

private:
    virtual sint32 relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY);
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;
    virtual void move(int x, int y);
    virtual Component::Ptr searchById(sint32 id);
    virtual Component::Ptr checkHit(int x, int y);

    boost::array<Panel::Ptr, Num_>      m_panels;
};

// ----------------------------------------------------------------------------
template <int Num_>
typename SplitPanel<Num_>::Ptr SplitPanel<Num_>::Create(
    ComponentId                         id)
{
    SplitPanel<Num_>* p = new SplitPanel<Num_>;
    for (int i = 0; i < Num_; ++i) p->m_panels[i] = Panel::Create(-1);
    return Ptr(p);
}

// ----------------------------------------------------------------------------
template <int Num_>
sint32 SplitPanel<Num_>::relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY)
{
    Component::relocate(parentLeft, parentWidth, locationY);
    // HACK:
    sint32 width = parentWidth / Num_;
    for (int i = 0; i < Num_; ++i)
    {
        static_cast<Component::Ptr>(m_panels[i])->relocate(parentLeft+(i*width), width, locationY);
    }
    location_.bottom = location_.top + getHeight();
    return location_.bottom;
}

// ----------------------------------------------------------------------------
template <int Num_>
void SplitPanel<Num_>::draw(Graphics& graphics)
{
    for (int i = 0; i < Num_; ++i)
    {
        static_cast<Component::Ptr>(m_panels[i])->draw(graphics);
    }
}

// ----------------------------------------------------------------------------
template <int Num_>
int SplitPanel<Num_>::getHeight() const
{
#undef max
    int maxHeight = 0;
    for (int i = 0; i < Num_; ++i)
    {
        maxHeight = std::max(static_cast<Component::Ptr>(m_panels[i])->getHeight(), maxHeight);
    }
    return maxHeight;
}

// ----------------------------------------------------------------------------
template <int Num_>
void SplitPanel<Num_>::move(int x, int y)
{
    using boost::bind;
    Component::move(x, y);
    std::for_each(m_panels.begin(), m_panels.end(), bind(&Component::move, _1, x, y));
}

// ----------------------------------------------------------------------------
template <int Num_>
Component::Ptr SplitPanel<Num_>::searchById(sint32 id)
{
    for (int i = 0; i < Num_; ++i)
    {
        Component::Ptr p = static_cast<Component::Ptr>(m_panels[i])->searchById(id);
        if (p.get() != 0) return p;
    }
    return Component::Ptr();
}

// ----------------------------------------------------------------------------
template <int Num_>
Component::Ptr SplitPanel<Num_>::checkHit(int x, int y)
{
    if (isPointInner(Point(x, y)))
    {
        for (int i = 0; i < Num_; ++i)
        {
            Component::Ptr p = static_cast<Component::Ptr>(m_panels[i])->checkHit(x, y);
            if (p != 0) return p;
        }
        return shared_from_this();
    }
    return Component::Ptr();
}

END_NAMESPACE_NYANCO_GUI
