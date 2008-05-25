#pragma once

/*!
    @file   Slider.h
    @author dasyprocta
 */

#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

class Slider : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Slider);

    struct Arg
    {
        Arg& min(float32 min)   { m_min = min;   return *this; }
        Arg& max(float32 max)   { m_max = max;   return *this; }
        Arg& step(float32 step) { m_step = step; return *this; }
        Arg& initial(float32 initial) { m_initial = initial; return *this; }

    private:
        float32                         m_min;
        float32                         m_max;
        float32                         m_step;
        float32                         m_initial;
    };

    static Ptr Create(Arg const& arg, ComponentId id = NonspecificId);

private:
    void draw(Graphics& graphics);
    int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);

    Arg                                 m_arg;
};

class ColorSlider : public Slider
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(ColorSlider);

    enum Type
    {
        IntR, IntG, IntB, IntA,
        IntH, IntS, IntV,
        FloatR, FloatG, FloatB, FloatA,
        FloatH, FloatS, FloatV,
    };

    struct Arg
    {
        Arg& type(Type type) { m_type = type; return *this; }

    private:
        Type                            m_type;
    };

private:
    void draw(Graphics& graphics);
    int getHeight() const;

    bool onMouseProcess(MouseCommand const& command);

    Arg                                 m_arg;
};

END_NAMESPACE_NYANCO_GUI
