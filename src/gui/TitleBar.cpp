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
    titleBar->onClosing_ = false;

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

    // close button
    Rect closeRegion = location_;
    closeRegion.right  -= 2;
    closeRegion.left    = closeRegion.right - 10;
    closeRegion.top    += 2;
    closeRegion.bottom -= 2;

    graphics.setColor(0xff444444);
    graphics.drawFillRect(closeRegion);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.right, closeRegion.top));
    graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.left, closeRegion.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(closeRegion.right, closeRegion.top+1), Point(closeRegion.right, closeRegion.bottom));
    graphics.drawLine(Point(closeRegion.left, closeRegion.bottom), Point(closeRegion.right, closeRegion.bottom));

    graphics.drawLine(Point(closeRegion.left+2, closeRegion.top+2), Point(closeRegion.right-2, closeRegion.bottom-2));
    graphics.drawLine(Point(closeRegion.left+2, closeRegion.bottom-2), Point(closeRegion.right-2, closeRegion.top+2));
}

// ----------------------------------------------------------------------------
void TitleBar::update()
{
    Mouse& mouse = InputDevice::GetInterface().getMouse();
    MouseCommand command;
    MouseCommand::Create(command, mouse);

    if (onClosing_)
    {
        if (command.onUpLeft)
        {
            // in button region
            Rect closeRegion = location_;
            closeRegion.right  -= 2;
            closeRegion.left    = closeRegion.right - 10;
            closeRegion.top    += 2;
            closeRegion.bottom -= 2;

            if (closeRegion.isInnerPoint(command.posX, command.posY))
            {
                WindowManager& wm = WindowManager::GetInterface();
                wm.detach(owner_);
            }
            onClosing_ = false;
        }
    }
    else if (onMoving_)
    {
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
        // in button region
        Rect closeRegion = location_;
        closeRegion.right  -= 2;
        closeRegion.left    = closeRegion.right - 10;
        closeRegion.top    += 2;
        closeRegion.bottom -= 2;

        if (closeRegion.isInnerPoint(command.posX, command.posY))
        {
            onClosing_ = true;
        }
        onMoving_ = true;
    }
}

END_NAMESPACE_NYANCO_GUI
