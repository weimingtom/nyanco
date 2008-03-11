/*!
    @file   TitleBar.cpp
    @author dasyprocta
 */

#include "TitleBar.h"
#include "Graphics.hpp"
#include "WindowManager.hpp"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
TitleBarPtr TitleBar::Create(
    std::string const&              name,
    std::string const&              caption,
    FramePtr                        owner)
{
    TitleBar* titleBar = new TitleBar;

    titleBar->setName(name);
    titleBar->caption_  = caption;
    titleBar->owner_    = owner;
    titleBar->onMoving_ = false;

    titleBar->setLocation(Rect(0, 0, 0, 14));

    return TitleBarPtr(titleBar);
}

// ----------------------------------------------------------------------------
void TitleBar::setCaption(std::string const& caption)
{
    caption_ = caption;
}

// ----------------------------------------------------------------------------
std::string const& TitleBar::getCaption() const
{
    return caption_;
}

// ----------------------------------------------------------------------------
void TitleBar::draw(Graphics& graphics)
{
    Rect caption = location_;
    caption.bottom = caption.top + 14;
    graphics.setColor(0xff6666ff);
    graphics.drawFillRect(caption);
    graphics.drawText(Point(caption.left+1, caption.top+1), caption_, 0xffeeeeee);
}

// ----------------------------------------------------------------------------
void TitleBar::update()
{
    if (onMoving_)
    {
        Mouse& mouse = InputDevice::GetInterface().getMouse();
        MouseCommand command;
        MouseCommand::Create(command, mouse);

        if (command.onUpLeft)
        {
            onMoving_ = false;
        }
        else if (command.onDownLeft)
        {
            owner_->move(command.moveX, command.moveY);
        }
    }
}

// ----------------------------------------------------------------------------
int TitleBar::getHeight() const
{
    return 14;
}

// ----------------------------------------------------------------------------
void TitleBar::onMouseProcess(MouseCommand const& command)
{
    if (command.onPushLeft)
    {
        onMoving_ = true;
    }
}

END_NAMESPACE_NYANCO_GUI
