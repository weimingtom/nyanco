#pragma once

/*!
    @file   Window.h
    @author dasyprocta
 */

#include "Panel.h"
#include "Event.h"
#include "Dock.h"
#include <boost/iterator_adaptors.hpp>

BEGIN_NAMESPACE_NYANCO_GUI

class Window
    : public EventServer,
      public boost::enable_shared_from_this<Window>
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(Window);

    virtual void moveTo(int x, int y);
    virtual void move(int x, int y);

    virtual void draw(Graphics& graphics) = 0;
    virtual void update() {}

    virtual void focus(Component::Ptr component);
    virtual void defocus();
    virtual void revertFocus();

    virtual Component::Ptr getFocusedComponent() const;
    virtual Component::Ptr getHitComponent(int x, int y) { return Component::Ptr(); }

    WindowId getId() const          { return m_id; }

    bool isPointInner(Point<sint32> const& point) const;

    void focusToNext();

protected:
    Component::Ptr getNextComponent(Component::Ptr comp);
    void setId(WindowId id)         { m_id = id; }
    Rect<sint32>& getLocation()             { return m_location; }
    Rect<sint32> const& getLocation() const { return m_location; }

    Container::Ptr getRootContainer() const { return m_panel; }
    //! ルートコンテナ
    Container::Ptr                      m_panel;

    Window() : m_panel(Panel::Create(-1)) {}

private:
    //! リターンキーを優先的にキャプチャするコンポーネント
    Component::WeakPtr                  m_returnComponent;
    Component::WeakPtr                  m_focusedComponent;
    Component::WeakPtr                  m_prevFocused;

    WindowId                            m_id;
    Rect<sint32>                                m_location;
};

class DockableWindow
    : public Window, public Dockable
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(DockableWindow);
};

END_NAMESPACE_NYANCO_GUI
