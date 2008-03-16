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
    FrameFinder(std::string const& name) : name_(name) {}
    bool operator()(FramePtr framePtr) const
    {
        return framePtr->getName() == name_;
    }
private:
    std::string name_;
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
    FramePtr                        framePtr)
{
    framePtrList_.push_front(framePtr);
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    FramePtr                        framePtr)
{
    killedFramePtrList_.push_back(framePtr);
}

// ------------------------------------------------------------------------
void WindowManager::detach(
    std::string const&              name)
{
    FramePtrList::iterator it = std::find_if(
        framePtrList_.begin(), framePtrList_.end(), FrameFinder(name));
    if (it != framePtrList_.end()) killedFramePtrList_.push_back(*it);
}

// ------------------------------------------------------------------------
FramePtr WindowManager::search(
    std::string const&              name)
{
    FramePtrList::iterator it = std::find_if(
        framePtrList_.begin(), framePtrList_.end(), FrameFinder(name));
    return (it != framePtrList_.end())? *it: FramePtr();
}

// ------------------------------------------------------------------------
void WindowManager::activate(
    FramePtr                        framePtr)
{
    framePtrList_.remove(framePtr);
    framePtrList_.push_front(framePtr);
}

// ------------------------------------------------------------------------
FramePtr WindowManager::getActiveWindow() const
{
    return framePtrList_.back();
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
        bind(&Frame::draw, _1, ref(*graphics_)));

    // draw context menu
    contextMenu_->draw(*graphics_);
}

// ------------------------------------------------------------------------
void WindowManager::update()
{
    // invoke event
    std::for_each(framePtrList_.begin(), framePtrList_.end(), bind(&Frame::invokeEvent, _1));
    
    // input
    {
        InputDevice& input = InputDevice::GetInterface();

        onMouseProcess(input.getMouse());
        onKeyboardProcess(input.getKeyboard());
    }

    // update
    std::for_each(framePtrList_.begin(), framePtrList_.end(), bind(&Frame::update, _1));
    contextMenu_->update();

    // kill frames
    foreach (FramePtr frame, killedFramePtrList_)
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
    using boost::bind;

    int x, y;
    mouse.getPosition(x, y);

    MouseCommand command;
    MouseCommand::Create(command, mouse);

    bool frameHit = false;
    foreach (FramePtr frame, framePtrList_)
    {
        ComponentPtr p = frame->checkHit(x, y);
        if (p.get() != 0)
        {
            frameHit = true;

            if (command.onPushLeft) activate(frame);
            p->onMouseProcess(command);
            break;
        }
    }

    if (!frameHit && command.onPushRight)
    {
        contextMenu_->visible(x, y);
    }
    else if (command.onPushRight || command.onPushLeft)
    {
        contextMenu_->invisible();
    }
}

// ------------------------------------------------------------------------
void WindowManager::onKeyboardProcess(Keyboard const& keyboard)
{

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
