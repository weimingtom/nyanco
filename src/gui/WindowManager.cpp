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

namespace nyanco { namespace gui { namespace impl
{

    WindowManager* WindowManager::myPtr_ = 0;

    // ------------------------------------------------------------------------
    void WindowManager::drawText(
        Point const&                    point,
        std::string const&              text,
        Color                           color)
    {
        graphics_->drawText(point, text, color);
    }

    // ------------------------------------------------------------------------
    void WindowManager::attach(
        Frame*                          framePtr)
    {
        frameList_.push_back(reinterpret_cast<Frame*>(framePtr));
    }

    // ------------------------------------------------------------------------
    void WindowManager::detach(
        Frame*                          framePtr)
    {
        // UNDONE:
    }

    // ------------------------------------------------------------------------
    void WindowManager::detach(
        std::string const&              name)
    {
        // UNDONE:
    }

    // ------------------------------------------------------------------------
    Frame* WindowManager::search(
        std::string const&              name)
    {
        // UNDONE:
        return 0;
    }

    // ------------------------------------------------------------------------
    Frame* WindowManager::getActiveWindow() const
    {
        // UNDONE:
        return 0;
    }

    // ------------------------------------------------------------------------
    void WindowManager::draw()
    {
#if 0
        using boost::bind;

        std::for_each(
            frameList_.begin(), frameList_.end(),
            bind(&Frame::draw, _1, *graphics_));
#endif
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
        if (mouse.onButtonPush(Mouse::Button::Left))
        {
            // UNDONE: ヒットコンポーネントのチェック
        }
        else if (mouse.onButtonUp(Mouse::Button::Left))
        {
            // UNDONE: ヒットコンポーネントのチェック
        }
        else if (mouse.onButtonDown(Mouse::Button::Left))
        {
            // UNDONE: ドラッグ処理
        }
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
