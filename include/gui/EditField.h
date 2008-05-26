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
class TextEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(TextEdit);

    NYANCO_GUI_ARG_DEF_BEGIN(TextEdit)
#define PARAM_SEQ                               \
        ((sint32,       maxLength,      256))   \
        ((std::string,  initial,        ""))    \
        ((std::string,  invalidChara,   ""))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(TextEdit)

    static TextEdit::Ptr Create(
        ComponentId                     id);

    std::string const& get() const      { return m_text; }
    void set(std::string const& text)   { m_text = text; }

    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);
    bool onKeyboardProcess(KeyboardCommand const& command);

private:
    void setCaret(sint32 x, sint32 y);

    std::string                         m_text;
    sint32                              m_caret;
    bool                                m_edit;
    boost::timer                        m_timer;

    TextEdit() : m_edit(false), m_caret(0) {}
};

// ============================================================================
class IntEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(IntEdit);

    NYANCO_GUI_ARG_DEF_BEGIN(IntEdit)
#define PARAM_SEQ                       \
        ((sint32,   min,      0))     \
        ((sint32,   max,      1024))  \
        ((sint32,   step,     1))     \
        ((sint32,   initial,  0))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(IntEdit)

private:
    Arg<>                               m_arg;
};

// ============================================================================
class FloatEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(FloatEdit);

    NYANCO_GUI_ARG_DEF_BEGIN(FloatEdit)
#define PARAM_SEQ                       \
        ((float32,  min,      0.f))   \
        ((float32,  max,      1024.f))\
        ((float32,  step,     1.f))   \
        ((float32,  initial,  0.f))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(FloatEdit)

private:
    Arg<>                               m_arg;
};

END_NAMESPACE_NYANCO_GUI
