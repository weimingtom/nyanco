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

    NYANCO_GUI_ARG_DEF_BEGIN(Rollup)
#define PARAM_SEQ               \
        ((std::string,  label))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(Rollup)

    static Rollup::Ptr Create(
        Arg<> const&                    arg,
        ComponentId                     id = NonspecificId);

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    virtual bool onMouseProcess(MouseCommand const& command);
    virtual bool onKeyboardProcess(KeyboardCommand const& command);

    virtual Component::Ptr getFirstComponent() const;
    virtual Component::Ptr getNextComponent(Component::ConstPtr component) const;

    Rollup() : m_collapsed(false) {}

    Arg<>                               m_arg;
    bool                                m_collapsed;
};

END_NAMESPACE_NYANCO_GUI
