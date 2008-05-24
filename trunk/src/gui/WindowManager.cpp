/*!
    @file   WindowManager.cpp
    @author dasyprocta
 */

#include "WindowManager.hpp"
#include "Frame.h"
#include "ContextMenu.h"
#include "Graphics.hpp"
#include "afx/InputDevice.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI_IMPL

WindowManager* WindowManager::myPtr_ = 0;

class FrameFinder
{
public:
    FrameFinder(ComponentId id) : m_id(id) {}
    bool operator()(Window::Ptr framePtr) const
    {
        return framePtr->getId() == m_id;
    }
private:
    ComponentId                         m_id;
};


// ------------------------------------------------------------------------
void WindowManager::drawText(
    Point const&                    point,
    std::string const&              text,
    Color                           color)
{
    FontInfo const& fontInfo = graphics_->getFontInfo();
    textList_.push_back(Text(Point(point.x * fontInfo.charaWidth, point.y * fontInfo.charaHeight), text, color));
}

// ------------------------------------------------------------------------
void WindowManager::attach(
    Window::Ptr                        framePtr)
{
    if (!isWindowExist(framePtr))
    {
        setFloatingWindow(framePtr);
    }
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    Window::Ptr                        framePtr)
{
    WindowList::iterator it = std::find(m_windowList.begin(), m_windowList.end(), framePtr);
    if (it != m_windowList.end())
    {
        m_killedWindowList.push_back(*it);

        WindowList::iterator it2 = std::find(m_dockingWindowList.begin(), m_dockingWindowList.end(), *it);
        if (it2 != m_dockingWindowList.end())
        {
            m_dockManager->getRoot()->undock(boost::shared_static_cast<DockableWindow>(*it2));
            m_dockingWindowList.remove(*it);
        }
        else m_floatingWindowList.remove(*it);
    }
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    ComponentId                         id)
{
    WindowList::iterator it = std::find_if(m_windowList.begin(), m_windowList.end(), FrameFinder(id));
    if (it != m_windowList.end())
    {
        m_killedWindowList.push_back(*it);

        WindowList::iterator it2 = std::find(m_dockingWindowList.begin(), m_dockingWindowList.end(), *it);
        if (it2 != m_dockingWindowList.end())
        {
            m_dockManager->getRoot()->undock(boost::shared_static_cast<DockableWindow>(*it2));
            m_dockingWindowList.remove(*it);
        }
        else m_floatingWindowList.remove(*it);
    }
}

// ------------------------------------------------------------------------
Window::Ptr WindowManager::search(
    ComponentId                         id)
{
    WindowList::iterator it = std::find_if(m_windowList.begin(), m_windowList.end(), FrameFinder(id));
    return (it != m_windowList.end())? *it: Window::Ptr();
}

// ------------------------------------------------------------------------
void WindowManager::activate(
    Window::Ptr                        framePtr)
{
    WindowList::const_iterator it = std::find(m_windowList.begin(), m_windowList.end(), framePtr);
    if (it != m_windowList.end())
    {
        if (Window::Ptr active = m_activeWindow.lock())
            active->defocus();

        // ドッキングされてなければ一番手前に移動
        WindowList::iterator it2 = std::find(m_dockingWindowList.begin(), m_dockingWindowList.end(), *it);
        if (it2 == m_dockingWindowList.end())
        {
            m_floatingWindowList.remove(framePtr);
            m_floatingWindowList.push_front(framePtr);
        }
        m_activeWindow = framePtr;
    }
}

// ------------------------------------------------------------------------
Window::Ptr WindowManager::getActiveWindow() const
{
    return m_activeWindow.lock();
}

// ------------------------------------------------------------------------
ContextMenu::Ptr WindowManager::getContextMenu() const
{
    return contextMenu_;
}

// ----------------------------------------------------------------------------
void WindowManager::setContextMenu(ContextMenu::Ptr menu)
{
    contextMenu_ = menu;
}

// ----------------------------------------------------------------------------
Dock::Ptr WindowManager::dock(
    DockableWindow::Ptr                 wnd,
    Dock::Type                          type)
{
    return dock(wnd, type, m_dockManager->getRoot());
}

// ----------------------------------------------------------------------------
Dock::Ptr WindowManager::dock(
    DockableWindow::Ptr                 wnd,
    Dock::Type                          type,
    Dock::Ptr                           dock)
{
    if (isDockingWindow(wnd))
    {
        // UNDONE: ウィンドウのドックを返すようにする
        return Dock::Ptr();
    }

    if (isWindowExist(wnd))
    {
        m_floatingWindowList.remove(wnd);
    }
    else
    {
        m_windowList.push_back(wnd);
    }
    m_dockingWindowList.push_back(wnd);
    return dock->dock(wnd, type);
}

// ----------------------------------------------------------------------------
void WindowManager::undock(
    DockableWindow::Ptr                 wnd)
{
    // TODO:
}

// ------------------------------------------------------------------------
void WindowManager::draw()
{
    using boost::bind;
    using boost::ref;

    // draw text list
    std::for_each(
        textList_.begin(), textList_.end(),
        bind(&Text::draw, _1, ref(*graphics_)));
    textList_.clear();

    // draw dock
    m_dockManager->draw(*graphics_);

    // draw frame list
    std::for_each(
        m_floatingWindowList.rbegin(), m_floatingWindowList.rend(),
        bind(&Window::draw, _1, ref(*graphics_)));

    // draw context menu
    contextMenu_->draw(*graphics_);
}

// ------------------------------------------------------------------------
void WindowManager::update()
{
    // invoke event
    std::for_each(m_windowList.begin(), m_windowList.end(), bind(&Window::invokeHandler, _1));
    
    // input
    {
        InputDevice& input = InputDevice::GetInterface();
        onInputProcess(input.getKeyboard(), input.getMouse());
    }

    // update
    m_dockManager->update(m_clientRect, m_windowRect);
    std::for_each(m_windowList.begin(), m_windowList.end(), bind(&Window::update, _1));
    contextMenu_->update();

    // kill frames
    foreach (Window::Ptr frame, m_killedWindowList)
    {
        m_windowList.remove(frame);
    }
    m_killedWindowList.clear();
}

// ------------------------------------------------------------------------
void WindowManager::initialize(
    LPDIRECT3DDEVICE9           devicePtr)
{
    m_dockManager.reset(new DockManager);
    graphics_.reset(new Graphics(devicePtr));
    contextMenu_.reset(new ContextMenu);
}

// ------------------------------------------------------------------------
void WindowManager::finalize()
{
    
}

// ------------------------------------------------------------------------
void WindowManager::onInputProcess(Keyboard const& keyboard, Mouse const& mouse)
{
    KeyboardCommand keyboardCommand;
    KeyboardCommand::Create(keyboardCommand, const_cast<Keyboard&>(keyboard));

    MouseCommand mouseCommand;
    MouseCommand::Create(mouseCommand, mouse);

    // キーボードキャプチャコンポーネントが存在
    if (Component::Ptr p = m_capturedKeyboard.lock())
    {
        if (!p->onKeyboardProcess(keyboardCommand))
        {
            m_capturedKeyboard.reset();
        }
    }
    // システムキーボード操作
    else
    {
        Window::Ptr frame = getActiveWindow();
        if (frame.get() != 0)
        {
            if (Component::Ptr p = frame->getFocusedComponent())
            {
                if (p->onKeyboardProcess(keyboardCommand))
                {
                    m_capturedKeyboard = p;
                }
            }
        }
    }

    // マウスキャプチャコンポーネントが存在
    if (ComponentPtr p = m_capturedMouse.lock())
    {
        if (!p->onMouseProcess(mouseCommand))
        {
            // キャプチャを解除
            m_capturedMouse.reset();
        }
    }
    // システムマウス操作
    else
    {
        Point point(mouseCommand.posX, mouseCommand.posY);

        Window::Ptr     hitFrame;
        Component::Ptr  hitComponent;
        // 浮動フレームのヒットチェック
        foreach (Window::Ptr frame, m_floatingWindowList)
        {
            if (frame->isPointInner(point))
            {
                hitFrame = frame;
                break;
            }
        }
        // ドックのヒットチェック
        if (hitFrame.get() == 0)
        {
            Dock::Ptr dock = m_dockManager->getDock(point);
            if (dock.get() != 0)
            {
                Dockable::Ptr p = dock->getDockee();
                Rect rect;
                p->getDockableRect(rect);
                if (rect.isInnerPoint(point.x, point.y))
                    hitFrame = boost::shared_static_cast<Frame<> >(p);
                else
                    hitComponent = dock;
            }
        }
        // フレームヒット時処理
        if (mouseCommand.onPushLeft)
        {
            // ヒットコンポーネントが存在
            if (hitComponent.get() == 0 && hitFrame.get() != 0 )
            {
                hitComponent = hitFrame->getHitComponent(point.x, point.y);
                activate(hitFrame);
                hitFrame->focus(hitComponent);
            }
            // 存在しない
            else if (hitFrame && !hitComponent)
            {
                hitFrame->revertFocus();
            }

            if (hitComponent.get() != 0 && hitComponent->onMouseProcess(mouseCommand))
            {
                // キャプチャを設定
                m_capturedMouse = hitComponent;
            }
        }

        if (!hitFrame.get() && mouseCommand.onButtonDown)
        {
            m_activeWindow.reset();
            foreach (Window::Ptr frame, m_windowList)
            {
                frame->defocus();
            }
        }

        if (!hitFrame.get() && mouseCommand.onPushRight)
        {
            contextMenu_->visible(mouseCommand.posX, mouseCommand.posY);
        }
        else if (mouseCommand.onPushRight || mouseCommand.onPushLeft)
        {
            contextMenu_->invisible();
        }
    }
}

// ------------------------------------------------------------------------
WindowManager& WindowManager::GetImplement()
{
    if (WindowManager::myPtr_ == 0)
    {
        WindowManager::myPtr_ = new WindowManager;
    }
    return *WindowManager::myPtr_;
}

END_NAMESPACE_NYANCO_GUI_IMPL
BEGIN_NAMESPACE_NYANCO_GUI

// ------------------------------------------------------------------------
WindowManager& WindowManager::GetInterface()
{
    if (impl::WindowManager::myPtr_ == 0)
    {
        impl::WindowManager::myPtr_ = new impl::WindowManager;
    }
    return *impl::WindowManager::myPtr_;
}

END_NAMESPACE_NYANCO_GUI
