#pragma once

/*!
    @file   Rollup.h
    @author dasyprocta
 */

#include "Container.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Rollup : public Container
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Rollup);

    static Rollup::Ptr Create(
        ComponentId                     id,
        std::string const&              label);

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    virtual bool onMouseProcess(MouseCommand const& command);

    Rollup() : m_collapsed(false) {}

    std::string                         m_label;
    bool                                m_collapsed;
};

END_NAMESPACE_NYANCO_GUI
