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
    static boost::shared_ptr<Button> Create(
        ComponentId                     id,
        std::string const&              text);

    enum Event
    {
        PushEvent,
    };

private:
    // override
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);

    std::string                     caption_;
    bool                            pushed_;

    Button() : pushed_(false) {}
};

typedef boost::shared_ptr<Button> ButtonPtr;

END_NAMESPACE_NYANCO_GUI
