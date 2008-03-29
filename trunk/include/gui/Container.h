#pragma once

/*!
    @file   Container.h
    @author dasyprocta
 */

#include "Component.h"
#include <list>

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class Container : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Container);

    enum Layout
    {
        VerticalLayout,
        HorizontalLayout,
    };

    void attach(
        ComponentPtr                componentPtr);

    void detach(
        ComponentPtr                componentPtr);

protected:
    virtual void draw(Graphics& graphcis) = 0;
    virtual ComponentPtr checkHit(int x, int y);
    virtual void resize(int parentWidth);
    void setMargin(Rect const& margin);

protected:
    virtual void move(int x, int y) { Component::move(x, y); }
    virtual void update() { Component::update(); }

    Rect                            margin_;
    Layout                          m_layout;

    typedef std::list<ComponentPtr> ComponentList;
    ComponentList                   componentList_;
};

// ============================================================================
class VerticalLayout : public Container
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(VerticalLayout);

protected:
    virtual void draw(Graphics& graphics);
};

// ============================================================================
class HorizontalLayout : public Container
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(HorizontalLayout);

    void setLayout(sint32 column);
    VerticalLayout::Ptr get(sint32 id);

protected:
    virtual void draw(Graphics& graphics);

private:
    sint32                              m_numColumn;
};

END_NAMESPACE_NYANCO_GUI
