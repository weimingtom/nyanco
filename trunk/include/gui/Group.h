#pragma once

/*!
    @file   Group.h
    @author dasyprocta
 */

#include "Container.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Group : public Container
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Group);

    static Group::Ptr Create(
        ComponentId                     id,
        std::string const&              label);

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    std::string                         m_label;
};

END_NAMESPACE_NYANCO_GUI
