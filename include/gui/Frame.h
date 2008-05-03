#pragma once

/*!
    @file   Frame.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Container.h"
#include "TitleBar.h"
#include "Event.h"
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
    : public EventServer,
      public Container
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

    void focus(sint32 componentId);
    void focus(Component::Ptr component);
    void defocus();

private:
    void relocateChildren();
    void relocateY();
    ComponentPtr getHitComponent(int x, int y);

protected:
    Component::WeakPtr                  m_focusedComponent;
    TitleBar::Ptr                       m_titleBar;

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
