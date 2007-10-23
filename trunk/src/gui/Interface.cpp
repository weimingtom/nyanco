/*!
    @file   Interface.cpp
    @author dasyprocta
 */

#include "gui/Interface.h"
#include "afx/GraphicsDevice.h"
#include "afx/InputDevice.h"
#include "WindowManager.hpp"

namespace nyanco
{
    void GuiInterface::Implement()
    {
        GuiInterface::RegistFactory(gui::Interface::Create);
    }

namespace gui
{

    void Interface::onInitialize()
    {
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        GraphicsDevice& graphics    = GraphicsDevice::GetInterface();
        manager.initialize(graphics.getD3dDevice());
    }

    void Interface::onFinalize()
    {
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        manager.finalize();
    }

    void Interface::onUpdate()
    {
        // “ü—Í‚ÌŽæ“¾
        InputDevice& input = InputDevice::GetInterface();
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        manager.onMouseProcess(input.getMouse());
        manager.onKeyboardProcess(input.getKeyboard());

        manager.update();
    }

    void Interface::onDraw()
    {
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        manager.draw();
    }

} } // namespace nyanco::gui
