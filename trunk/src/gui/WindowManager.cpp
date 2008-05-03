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
    if (framePtrList_.empty()) return;
    framePtrList_.front()->defocus();
    framePtrList_.remove(framePtr);
    framePtrList_.push_front(framePtr);
}

// ------------------------------------------------------------------------
Frame<>::Ptr WindowManager::getActiveWindow() const
{
    if (framePtrList_.empty())
    {
        return Frame<>::Ptr();
    }
    return framePtrList_.front();
}

// ------------------------------------------------------------------------
ContextMenuPtr WindowManager::getContextMenu() const
{
    return contextMenu_;
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

        onMouseProcess(input.getMouse());
        onKeyboardProcess(input.getKeyboard());
    }

    // update
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
    graphics_.reset(new Graphics(devicePtr));
    contextMenu_.reset(new ContextMenu);
}

// ------------------------------------------------------------------------
void WindowManager::finalize()
{
    
}

// ------------------------------------------------------------------------
void WindowManager::onMouseProcess(Mouse const& mouse)
{
    MouseCommand command;
    MouseCommand::Create(command, mouse);

    // マウスキャプチャコンポーネントが存在
    if (ComponentPtr p = m_capturedMouse.lock())
    {
        if (!p->onMouseProcess(command))
        {
            // キャプチャを解除
            m_capturedMouse.reset();
        }
    }
    else
    {
        bool frameHit = false;
        foreach (Frame<>::Ptr frame, framePtrList_)
        {
            Component::Ptr p = frame->getHitComponent(command.posX, command.posY);
            if (p.get() != 0)
            {
                frameHit = true;
                if (command.onButtonDown)
                {
                    activate(frame);
                    frame->focus(p);
                    if (p->onMouseProcess(command))
                    {
                        // キャプチャを設定
                        m_capturedMouse = p;
                    }
                }
                break;
            }
        }
        if (!frameHit && command.onButtonDown)
        {
            foreach (Frame<>::Ptr frame, framePtrList_)
            {
                frame->defocus();
            }
        }

        if (!frameHit && command.onPushRight)
        {
            contextMenu_->visible(command.posX, command.posY);
        }
        else if (command.onPushRight || command.onPushLeft)
        {
            contextMenu_->invisible();
        }
    }
}

// ------------------------------------------------------------------------
void WindowManager::onKeyboardProcess(Keyboard const& keyboard)
{
    KeyboardCommand command;
    KeyboardCommand::Create(command, keyboard);

    // キーボードキャプチャコンポーネントが存在
    if (ComponentPtr p = m_capturedKeyboard.lock())
    {
        p->onKeyboardProcess(command);
    }
    else
    {
        Frame<>::Ptr frame = getActiveWindow();
        if (frame.get() == 0)
        {
            
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
