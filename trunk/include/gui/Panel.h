#pragma once

/*!
    @file   Panel.h
    @author dasyprocta
 */

#include "Container.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Panel : public Container
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Panel);

    static Panel::Ptr Create(
        ComponentId                     id);

private:
    virtual bool enableFocus() const { return false; }
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;
};

END_NAMESPACE_NYANCO_GUI
