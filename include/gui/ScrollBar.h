#pragma once

/*!
    @file   ScrollBar.h
    @author dasyprocta
 */

#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ============================================================================
class ScrollBar : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(ScrollBar);
    enum Type { Vertical, Horizontal };

    struct Owner
    {
        NYANCO_GUI_COMPONENT_TYPEDEF(Owner);

        //! スクロール方向のクライアント領域の長さ
        virtual sint32 getClientWidth() = 0;
        //! スクロール方向の内容領域の長さ
        virtual sint32 getContentWidth() = 0;
        virtual sint32 getOneContentSize() = 0;
        virtual sint32 getNumContents() = 0;

        sint32 getScrollBarOffset() const
        {
            return m_scrollClientOffset;
        }

    private:
        sint32                          m_scrollClientOffset;

        friend ScrollBar;
    };

    struct Arg
    {
        Arg& owner(Owner::Ptr owner) { m_owner = owner; return *this; }
        Arg& type(Type type) { m_type = type; return *this; }
        Owner::Ptr                      m_owner;
        Type                            m_type;
    };

    static ScrollBar::Ptr Create(
        ComponentId                     id,
        Arg const&                      arg);

    virtual void draw(Graphics& graphics);
    virtual void update();
    virtual bool onMouseProcess(MouseCommand const& mouse);

private:
    Arg                                 m_arg;
    float32                             m_scrollOffset;
    float32                             m_scrollSize;
    float32                             m_sliderSize;
    bool                                m_buttonDown[2];
    bool                                m_sliderDown;
    bool                                m_enableScroll;
};

END_NAMESPACE_NYANCO_GUI
