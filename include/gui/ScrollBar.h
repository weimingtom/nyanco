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

    struct Arg;

    /*!
        @brief  スクロールバー主

        スクロールバーを使用するコンポーネントはこのクラスを継承してください。
     */
    class Owner
    {
        NYANCO_GUI_COMPONENT_TYPEDEF(Owner);

        virtual void getScrolledClientRect(Rect& rect) const = 0;
        virtual void getScrolledContentSize(Size& size) const = 0;
        virtual void getScrolledUnitInclementSize(Size& size) const = 0;

        /*
        virtual sint32 getClientSize(Type type) const = 0;
        virtual sint32 getContentSize(Type type) const = 0;
        virtual sint32 getUnitInclementSize(Type type) const = 0;
        virtual bool isVHScrollEnabled() const { return false; }
        */

    protected:
        void createScrollBar(
            Owner::Ptr                  owner,
            Arg*                        verticalArg,
            Arg*                        horizontalArg);

        ScrollBar::Ptr getScrollBar(Type type) const
        {
            return m_scrollBar[type];
        }

        void getClientSizeWithScrollBar(Size& size) const;

        void updateScrollBar();
        void drawScrollBar(Graphics& g);

        bool onScrollBarMouseProcess(MouseCommand const& mouse);

        ScrollBar::Ptr                  m_scrollBar[2];

        friend ScrollBar;
    };

    struct Arg
    {
        Arg& type(Type type)        { m_type = type;        return *this; }
        Arg& disable(bool disable)  { m_disable = disable;  return *this; }
        Arg() : m_type(Vertical), m_disable(false) {}

    private:
        Type                            m_type;
        bool                            m_disable;

        friend ScrollBar;
    };

    static Ptr Create(Arg const& arg, Owner::Ptr owner, ComponentId id = NonspecificId);

    sint32 getContentOffset() const;
    bool isScrollEnabled() const;
    bool isVisible() const;

    virtual void draw(Graphics& graphics);
    virtual void update();
    virtual bool onMouseProcess(MouseCommand const& mouse);

private:
    Arg                                 m_arg;

    float32                             m_scrollOffset;
    float32                             m_scrollSize;
    float32                             m_sliderSize;
    sint32                              m_scrollClientOffset;

    bool                                m_buttonDown[2];
    bool                                m_sliderDown;
    bool                                m_enableScroll;

    Owner::WeakPtr                      m_owner;

    sint32 getClientSize() const;
    sint32 getContentSize() const;
    sint32 getUnitInclementSize() const;
    void calcButtonRect(Rect& b1, Rect& b2) const;
    void calcSliderRect(Rect& s) const;
};

END_NAMESPACE_NYANCO_GUI
