#pragma once

/*!
    @file   Button.h
    @author dasyprocta
 */

#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Button : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Button);

    static Button::Ptr Create(
        ComponentId                     id,
        std::string const&              text);

    struct Event
    {
        enum Type { Click, };
    };

private:
    // override
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);
    bool onKeyboardProcess(KeyboardCommand const& command);

    std::string                     caption_;
    bool                            pushed_;

    Button() : pushed_(false) {}
};

typedef boost::shared_ptr<Button> ButtonPtr;

END_NAMESPACE_NYANCO_GUI
