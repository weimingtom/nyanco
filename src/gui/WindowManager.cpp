/*!
    @file   WindowManager.cpp
    @author dasyprocta
 */

#include "WindowManager.hpp"
#include "Graphics.hpp"
#include "Frame.h"
#include "afx/InputDevice.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

namespace nyanco { namespace gui { namespace impl
{

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
    }

    // ------------------------------------------------------------------------
    void WindowManager::update()
    {
        // 入力の解析
        {
            InputDevice& input = InputDevice::GetInterface();

            onMouseProcess(input.getMouse());
            onKeyboardProcess(input.getKeyboard());
        }

        std::for_each(framePtrList_.begin(), framePtrList_.end(), bind(&Frame::update, _1));
    }

    // ------------------------------------------------------------------------
    void WindowManager::initialize(
        LPDIRECT3DDEVICE9           devicePtr)
    {
        graphics_ = new Graphics(devicePtr);
    }

    // ------------------------------------------------------------------------
    void WindowManager::finalize()
    {
        delete graphics_;
    }

    // ------------------------------------------------------------------------
    void WindowManager::onMouseProcess(Mouse const& mouse)
    {
        using boost::bind;

        int x, y;
        mouse.getPosition(x, y);

        MouseCommand command;
        MouseCommand::Create(command, mouse);

        static ComponentPtr prevHitComponent;

        foreach (FramePtr frame, framePtrList_)
        {
            ComponentPtr p = frame->checkHit(x, y);
            if (p.get() != 0)
            {
                if (command.onPushLeft) activate(frame);

                p->onMouseProcess(command);
                break;
            }
        }

#if 0
        if (mouse.onButtonPush(Mouse::Button::Left))
        {
            // UNDONE: ヒットコンポーネントのチェック
            int x, y;
            mouse.getPosition(x, y);

            std::for_each(
                framePtrList_.rbegin(), framePtrList_.rend(),
                bind(&Frame::checkHit, _1, x, y));
        }
        else if (mouse.onButtonUp(Mouse::Button::Left))
        {
            // UNDONE: ヒットコンポーネントのチェック
        }
        else if (mouse.onButtonDown(Mouse::Button::Left))
        {
            // UNDONE: ドラッグ処理
        }
#endif
    }

    // ------------------------------------------------------------------------
    void WindowManager::onKeyboardProcess(Keyboard const& keyboard)
    {

    }

#if 0
    // ------------------------------------------------------------------------
    void WindowManager::onMouseLeftPush()
    {
        // UNDONE: ヒットコンポーネントのチェック
    }

    // ------------------------------------------------------------------------
    void WindowManager::onMouseLeftUp()
    {
        // UNDONE: ヒットコンポーネントのチェック
    }

    // ------------------------------------------------------------------------
    void WindowManager::onMouseRightPush()
    {

    }

    // ------------------------------------------------------------------------
    void WindowManager::onMouseRightUp()
    {

    }
#endif

    // ------------------------------------------------------------------------
    void WindowManager::onKeyPush()
    {

    }

    // ------------------------------------------------------------------------
    void WindowManager::onKeyUp()
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

} } } // namespace nyanco::gui::impl

namespace nyanco { namespace gui
{

    // ------------------------------------------------------------------------
    WindowManager& WindowManager::GetInterface()
    {
        if (impl::WindowManager::myPtr_ == 0)
        {
            impl::WindowManager::myPtr_ = new impl::WindowManager;
        }
        return *impl::WindowManager::myPtr_;
    }

} } // namespace nyanco::gui
