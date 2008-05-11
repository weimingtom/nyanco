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
    bool operator()(Frame<>::Ptr framePtr) const
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
    Frame<>::Ptr                        framePtr)
{
    if (!isExistFrame(framePtr))
    {
        m_frameList.push_front(framePtr);
        m_eventServerList.push_back(framePtr);
    }
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    Frame<>::Ptr                        framePtr)
{
    killedFramePtrList_.push_back(framePtr);
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    ComponentId                         id)
{
    FramePtrList::iterator it = std::find_if(
        m_frameList.begin(), m_frameList.end(), FrameFinder(id));
    if (it != m_frameList.end()) killedFramePtrList_.push_back(*it);
}

// ------------------------------------------------------------------------
Frame<>::Ptr WindowManager::search(
    ComponentId                         id)
{
    FramePtrList::iterator it = std::find_if(
        m_frameList.begin(), m_frameList.end(), FrameFinder(id));
    return (it != m_frameList.end())? *it: Frame<>::Ptr();
}

// ------------------------------------------------------------------------
void WindowManager::activate(
    Frame<>::Ptr                        framePtr)
{
    FramePtrList::const_iterator it = std::find(m_frameList.begin(), m_frameList.end(), framePtr);
    if (it != m_frameList.end())
    {
        m_frameList.remove(framePtr);
        m_frameList.push_front(framePtr);
    }
    m_activeFrame = framePtr;
}

// ------------------------------------------------------------------------
Frame<>::Ptr WindowManager::getActiveWindow() const
{
    return m_activeFrame.lock();
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
    Frame<>::Ptr                        dockable,
    Dock::Type                          type)
{
    FramePtrList::const_iterator it =
        std::find(m_frameList.begin(), m_frameList.end(), dockable);
    if (it != m_frameList.end())
    {
        m_frameList.remove(dockable);
    }
    else
    {
        m_eventServerList.push_back(dockable);
    }
    return m_dockManager->getRoot()->dock(dockable, type);
}

// ----------------------------------------------------------------------------
void WindowManager::undock(
    Frame<>::Ptr                        dockable)
{
    if (m_dockManager->isDockableExist(dockable))
    {
        m_dockManager->getRoot()->undock(dockable);
        attach(dockable);
    }
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
        m_frameList.rbegin(), m_frameList.rend(),
        bind(&Frame<>::draw, _1, ref(*graphics_)));

    // draw context menu
    contextMenu_->draw(*graphics_);
}

// ------------------------------------------------------------------------
void WindowManager::update()
{
    // invoke event
    std::for_each(m_eventServerList.begin(), m_eventServerList.end(), bind(&EventServer::invokeHandler, _1));
    
    // input
    {
        InputDevice& input = InputDevice::GetInterface();
        onInputProcess(input.getKeyboard(), input.getMouse());
    }

    // update
    m_dockManager->update(m_clientRect, m_windowRect);
    std::for_each(m_frameList.begin(), m_frameList.end(), bind(&Frame<>::update, _1));
    contextMenu_->update();

    // kill frames
    foreach (Frame<>::Ptr frame, killedFramePtrList_)
    {
        m_frameList.remove(frame);
        m_eventServerList.remove(frame);
    }
    killedFramePtrList_.clear();
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
        Frame<>::Ptr frame = getActiveWindow();
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

        Frame<>::Ptr    hitFrame;
        Component::Ptr  hitComponent;
        // 浮動フレームのヒットチェック
        foreach (Frame<>::Ptr frame, m_frameList)
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
            if (hitComponent.get() == 0 && hitFrame.get() != 0 )
            {
                hitComponent = hitFrame->getHitComponent(point.x, point.y);
                activate(hitFrame);
                hitFrame->focus(hitComponent);
            }

            if (hitComponent.get() != 0 && hitComponent->onMouseProcess(mouseCommand))
            {
                // キャプチャを設定
                m_capturedMouse = hitComponent;
            }
        }

        if (!hitFrame.get() && mouseCommand.onButtonDown)
        {
            m_activeFrame.reset();
            foreach (Frame<>::Ptr frame, m_frameList)
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
