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
    virtual void attach(
        ComponentPtr                componentPtr);

    virtual void detach(
        ComponentPtr                componentPtr);

    virtual void detach(
        std::string const&          componentName);

protected:
    virtual void draw(Graphics& graphcis) = 0;
    virtual ComponentPtr checkHit(int x, int y);
    virtual void resize(int parentWidth);
    void setMargin(Rect const& margin);

protected:
    virtual void move(int x, int y) { Component::move(x, y); }
    virtual void update() { Component::update(); }

    Rect                            margin_;
    typedef std::list<ComponentPtr> ComponentList;
    ComponentList                   componentList_;
};

END_NAMESPACE_NYANCO_GUI
