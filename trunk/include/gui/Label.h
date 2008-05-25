#pragma once

/*!
    @file   Label.h
    @author dasyprocta
 */

#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Label : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Label);

    NYANCO_GUI_ARG_DEF_BEGIN(Label)
#define PARAM_SEQ \
        ((std::string,  text))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(Label)

    static Ptr Create(Arg<> const& arg, ComponentId id = NonspecificId);

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    Arg<>                               m_arg;
};

END_NAMESPACE_NYANCO_GUI
