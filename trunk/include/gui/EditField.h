#pragma once

/*!
    @file   EditField.h
    @author dasyprocta
 */

#include "Component.h"
#include <boost/timer.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class EditField : public Component
{
protected:
};

// ============================================================================
class TextField : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(TextField);

    struct Arg
    {
        sint32                          m_maxLength;

        Arg& maxLength(sint32 maxLength)    { m_maxLength = maxLength; return *this; }
    };

    static TextField::Ptr Create(
        ComponentId                     id);

    std::string const& get() const      { return m_text; }
    void set(std::string const& text)   { m_text = text; }

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);
    bool onKeyboardProcess(KeyboardCommand const& command);

    void setCaret(sint32 x, sint32 y);

    std::string                         m_text;
    sint32                              m_caret;
    bool                                m_edit;
    boost::timer                        m_timer;

    TextField() : m_edit(false), m_caret(-1) {}
};

// ============================================================================
class IntEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(IntEdit);
};

// ============================================================================
class FloatEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(FloatEdit);
};

END_NAMESPACE_NYANCO_GUI
