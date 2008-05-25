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

    NYANCO_GUI_ARG_DEF_BEGIN(TextField)
#define PARAM_SEQ                           \
        ((sint32,       maxLength))         \
        ((std::string,  initial))           \
        ((std::string,  invalidChara))
        NYANCO_GUI_ARG_PARAMS(PARAM_SEQ)
#undef PARAM_SEQ
    NYANCO_GUI_ARG_DEF_END(TextField)

    static TextField::Ptr Create(
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

    TextField() : m_edit(false), m_caret(0) {}
};

// ============================================================================
class IntEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(IntEdit);

    struct Arg
    {
        sint32                          m_min;
        sint32                          m_max;
        sint32                          m_step;
        sint32                          m_initial;

        Arg& min(sint32 min)    { m_min = min; return *this; }
        Arg& max(sint32 max)    { m_max = max; return *this; }
        Arg& step(sint32 step)  { m_step = step; return *this; }
        Arg& initial(sint32 initial) { m_initial = initial; return *this; }
    };

private:
    Arg                                 m_arg;
};

// ============================================================================
class FloatEdit : public EditField
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(FloatEdit);

    struct Arg
    {
        float32                         m_min;
        float32                         m_max;
        float32                         m_step;
        float32                         m_initial;

        Arg& min(float32 min)    { m_min = min; return *this; }
        Arg& max(float32 max)    { m_max = max; return *this; }
        Arg& step(float32 step)  { m_step = step; return *this; }
        Arg& initial(float32 initial) { m_initial = initial; return *this; }
    };

private:
    Arg                                 m_arg;
};

END_NAMESPACE_NYANCO_GUI
