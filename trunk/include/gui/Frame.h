#pragma once

/*!
    @file   Frame.h
    @author dasyprocta
 */

#include "Window.h"
#include "TitleBar.h"
#include "Panel.h"
#include "Dock.h"
#include <vector>
#include <boost/tuple/tuple.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class WindowManager;
class Graphics;
template <typename Derived_> class Frame;

namespace impl { class WindowManager; }

// ============================================================================
template <>
class Frame<void>
    : public DockableWindow
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Frame<void>);

    struct Arg
    {
        std::string                     m_caption;
        sint32                          m_width;
        sint32                          m_height;

        Arg& caption(std::string const& caption) { m_caption = caption; return *this; }
        Arg& width(sint32 width) { m_width = width; return *this; }
        Arg& height(sint32 height) { m_height = height; return *this; }
    };

    void moveTo(int x, int y);
    void move(int x, int y);

    void create(
        Frame<void>::Ptr                frame,
        ComponentId                     id,
        std::string const&              caption,
        uint32                          width,
        uint32                          height);
    void draw(Graphics& graphics);
    void update();

    void attach(Component::Ptr component);
    void detach(Component::Ptr component);

    // Dockable �C���^�[�t�F�[�X����
    void getDockableRect(Rect<sint32>& rect);
    void setDockableRect(Rect<sint32> const& rect);
    void drawDockable(Graphics& graphics);
    void onDock();
    void onUndock();
    void onUpdate() { update(); }

private:
    void relocateChildren();
    void relocateY();
    Component::Ptr getHitComponent(int x, int y);

protected:
    TitleBar::Ptr                       m_titleBar;

    Rect<sint32>                                margin_;
    Rect<sint32>                                m_prevLocation;

    friend impl::WindowManager;
};

// ============================================================================
template <typename Derived_ = void>
class Frame
    : public Frame<>
{
public:
    typedef Derived_                    Derived;
    typedef Frame<Derived>              Super;
    NYANCO_GUI_COMPONENT_TYPEDEF(Derived_);

    static boost::shared_ptr<Derived_> Create(
        WindowId                        id,
        Frame<>::Arg const&             arg)
    {
        Derived::Ptr derived(new Derived);
        derived->create(
            derived,
            id,
            arg.m_caption,
            arg.m_width,
            arg.m_height);
        return Derived::Ptr(derived);
    }

#if 0
    template <typename Arg0_>
    static typename Derived::Ptr Create(
        WindowId                        id,
        Frame<>::Arg const&             arg,
        boost::tuple<Arg0_>             ctorArg);

    template <typename Arg0_, typename Arg1_>
    static typename Derived::Ptr Create(
        WindowId                        id,
        Frame<>::Arg const&             arg,
        boost::tuple<Arg0_, Arg1_>      ctorArg);

    template <typename Arg0_, typename Arg1_, typename Arg2_>
    static typename Derived::Ptr Create(
        WindowId                        id,
        Frame<>::Arg const&             arg,
        boost::tuple<Arg0_, Arg1_, Arg2_> ctorArg);
#endif
};

END_NAMESPACE_NYANCO_GUI
