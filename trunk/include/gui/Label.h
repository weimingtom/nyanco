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

    template <typename Class_ = Label>
    struct Arg
    {
        typedef Class_ Class;
        typename Class::Arg<Class>& text(std::string const& text) { m_text = text; return *static_cast<typename Class::Arg<Class>*>(this); }

        std::string                     m_text;
    };

    static Ptr Create(Arg<> const& arg, ComponentId id = NonspecificId);

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    Arg<>                               m_arg;
};

END_NAMESPACE_NYANCO_GUI
