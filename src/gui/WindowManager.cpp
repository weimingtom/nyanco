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
    framePtrList_.push_front(framePtr);
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
        framePtrList_.begin(), framePtrList_.end(), FrameFinder(id));
    if (it != framePtrList_.end()) killedFramePtrList_.push_back(*it);
}

// ------------------------------------------------------------------------
Frame<>::Ptr WindowManager::search(
    ComponentId                         id)
{
    FramePtrList::iterator it = std::find_if(
        framePtrList_.begin(), framePtrList_.end(), FrameFinder(id));
    return (it != framePtrList_.end())? *it: Frame<>::Ptr();
}

// ------------------------------------------------------------------------
void WindowManager::activate(
    Frame<>::Ptr                        framePtr)
{
    framePtrList_.remove(framePtr);
    framePtrList_.push_front(framePtr);
    m_activeFrame = framePtr;
}

// ------------------------------------------------------------------------
Frame<>::Ptr WindowManager::getActiveWindow() const
{
    return m_activeFrame.lock();
}

// ------------------------------------------------------------------------
ContextMenuPtr WindowManager::getContextMenu() const
{
    return contextMenu_;
}

// ----------------------------------------------------------------------------
Dock::Ptr WindowManager::dock(
    Dockable::Ptr                       dockable,
    Dock::Type                          type)
{
    detach(boost::shared_dynamic_cast<Frame<> >(dockable));
    return m_dockManager->getRoot()->dock(dockable, type);
}

// ----------------------------------------------------------------------------
void WindowManager::undock(
    Dockable::Ptr                       dockable)
{
    m_dockManager->getRoot()->undock(dockable);
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
        framePtrList_.rbegin(), framePtrList_.rend(),
        bind(&Frame<>::draw, _1, ref(*graphics_)));

    // draw context menu
    contextMenu_->draw(*graphics_);
}

// ------------------------------------------------------------------------
void WindowManager::update()
{
    // invoke event
    std::for_each(framePtrList_.begin(), framePtrList_.end(), bind(&Frame<>::invokeHandler, _1));
    
    // input
    {
        InputDevice& input = InputDevice::GetInterface();
        onInputProcess(input.getKeyboard(), input.getMouse());
    }

    // update
    m_dockManager->update(m_clientRect, m_windowRect);
    std::for_each(framePtrList_.begin(), framePtrList_.end(), bind(&Frame<>::update, _1));
    contextMenu_->update();

    // kill frames
    foreach (Frame<>::Ptr frame, killedFramePtrList_)
    {
        framePtrList_.remove(frame);
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
        foreach (Frame<>::Ptr frame, framePtrList_)
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
            foreach (Frame<>::Ptr frame, framePtrList_)
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
