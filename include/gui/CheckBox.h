#pragma once

/*!
    @file   CheckBox.h
    @author dasyprocta
 */

#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

class CheckBox : public Component
{
public:
    typedef boost::shared_ptr<CheckBox> Ptr;
    typedef boost::weak_ptr<CheckBox>   WeakPtr;

    struct Event
    {
        enum Type { Click, };
    };

    static Ptr Create(
        ComponentId                 id,
        std::string const&          text,
        bool                        check = false);

    void setCheck(bool check);
    bool getCheck() const;

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);

    CheckBox() : m_pushed(false) {}

    std::string                     text_;
    bool                            m_checked;
    bool                            m_pushed;
};

END_NAMESPACE_NYANCO_GUI
