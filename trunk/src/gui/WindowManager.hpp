#pragma once

/*!
    @file   WindowManager.hpp
    @author dasyprocta
 */

#include "gui_impl_base.hpp"
#include "gui/WindowManager.h"
#include "Graphics.hpp"
#include "Component.h"
#include "Dock.h"
#include <d3d9.h>
#include <vector>
#include <list>
#include <boost/scoped_ptr.hpp>

BEGIN_NAMESPACE_NYANCO

class Mouse;
class Keyboard;

END_NAMESPACE_NYANCO
BEGIN_NAMESPACE_NYANCO_GUI

template <typename Type_> class Frame;
class ContextMenu;
class Interface;

END_NAMESPACE_NYANCO_GUI
BEGIN_NAMESPACE_NYANCO_GUI_IMPL

class WindowManager : public nyanco::gui::WindowManager
{
    virtual void drawText(
        Point const&                point,
        std::string const&          text,
        Color                       color);

    virtual void attach(
        nyanco::gui::Window::Ptr        framePtr);

    virtual void detach(
        Window::Ptr                     framePtr);

    virtual void detach(
        ComponentId                     id);

    virtual Window::Ptr search(
        ComponentId                     id);

    virtual void activate(
        Window::Ptr                     framePtr);

    virtual Window::Ptr getActiveWindow() const;

    virtual ContextMenuPtr getContextMenu() const;
    virtual void setContextMenu(ContextMenu::Ptr menu);

    virtual Rect const& getClientRect() const { return m_clientRect; }
    virtual Rect const& getViewRect() const { return m_windowRect; }

    Dock::Ptr getRootDock() const { return m_dockManager->getRoot(); }

    virtual Dock::Ptr dock(
        DockableWindow::Ptr             wnd,
        Dock::Type                      type);

    virtual Dock::Ptr dock(
        DockableWindow::Ptr             wnd,
        Dock::Type                      type,
        Dock::Ptr                       dock);

    virtual void undock(
        DockableWindow::Ptr             wnd);

    void initialize(
        LPDIRECT3DDEVICE9               devicePtr);

    void finalize();

    void update();

    void draw();

    void setClientRect(Rect const& rect) { m_clientRect = rect; }

    static WindowManager& GetImplement();

    void onInputProcess(Keyboard const& keyboard, Mouse const& mouse);

private:
    class InputState
    {
    private:
        Point                       clickPoint_;
    };

    class Text
    {
    public:
        Text(
            Point const&            point,
            std::string const&      text,
            Color                   color)
            : point_(point), text_(text), color_(color) {}

        void draw(Graphics& graphics)
        {
//            graphics.drawText(point_, text_, color_);
        }

    private:
        Point                       point_;
        std::string                 text_;
        Color                       color_;
    };

    typedef std::list<Window::Ptr>  WindowList;
    typedef std::vector<Text>       TextList;

    //! 全ウィンドウリスト
    WindowList                      m_windowList;
    Window::WeakPtr                 m_activeWindow;
    //! 浮動ウィンドウリスト
    WindowList                      m_floatingWindowList;
    //! ドッキングウィンドウリスト
    WindowList                      m_dockingWindowList;

    //! テキスト
    TextList                        textList_;
    //! メニュー
    ContextMenu::Ptr                contextMenu_;

    Component::WeakPtr              m_capturedMouse;
    Component::WeakPtr              m_capturedKeyboard;

    WindowList                      m_killedWindowList;

    Rect                            m_windowRect;
    Rect                            m_clientRect;

    boost::scoped_ptr<DockManager>  m_dockManager;

    boost::scoped_ptr<Graphics>     graphics_;

    static WindowManager*           myPtr_;

    friend Interface;
    friend nyanco::gui::WindowManager;

    bool isWindowExist(Window::Ptr frame) const
    {
        WindowList::const_iterator it = std::find(m_windowList.begin(), m_windowList.end(), frame);
        return it != m_windowList.end();
    }

    bool isFloatingWindow(Window::Ptr window) const
    {
        WindowList::const_iterator it = std::find(m_floatingWindowList.begin(), m_floatingWindowList.end(), window);
        return it != m_floatingWindowList.end();
    }

    bool isDockingWindow(Window::Ptr window) const
    {
        WindowList::const_iterator it = std::find(m_dockingWindowList.begin(), m_dockingWindowList.end(), window);
        return it != m_dockingWindowList.end();
    }

    void setFloatingWindow(Window::Ptr window)
    {
        m_windowList.push_front(window);
        m_floatingWindowList.push_front(window);
    }

    void setDockableWindow(DockableWindow::Ptr window)
    {
        m_windowList.push_back(window);
        m_dockingWindowList.push_back(window);
    }
};

END_NAMESPACE_NYANCO_GUI_IMPL
