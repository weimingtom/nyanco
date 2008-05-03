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
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(EditField);
};

// ============================================================================
class TextField : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(TextField);

    static TextField::Ptr Create(
        ComponentId                     id);

    std::string const& get() const      { return m_text; }
    void set(std::string const& text)   { m_text = text; }

private:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);
    void onKeyboardProcess(KeyboardCommand const& command);

    void setCaret(sint32 x, sint32 y);

    std::string                         m_text;
    sint32                              m_caret;
    bool                                m_edit;
    boost::timer                        m_timer;

    TextField() : m_edit(false), m_caret(-1) {}
};

END_NAMESPACE_NYANCO_GUI
