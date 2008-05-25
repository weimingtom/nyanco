#pragma once

/*!
    @file   Container.h
    @author dasyprocta
 */

#include "Component.h"
#include <list>

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class Container : public Component, public ComponentIterator
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

    void setEventServer(EventServer* server);

    virtual Component::Ptr searchById(int id);

    Component::Ptr getFirstComponent() const;
    Component::Ptr getLastComponent() const;
    Component::Ptr getNextComponent(Component::ConstPtr component) const;
    Component::Ptr getPrevComponent(Component::ConstPtr component) const;

protected:
    sint32 relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY);
    void move(int x, int y);

    virtual void draw(Graphics& graphcis) = 0;
    virtual Component::Ptr checkHit(int x, int y);
//    virtual void resize(int parentWidth);
    void setMargin(Rect const& margin);

protected:
    virtual void update();

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
