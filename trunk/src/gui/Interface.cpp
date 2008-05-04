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
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        manager.update();
    }

    void Interface::onDraw()
    {
        impl::WindowManager&  manager     = impl::WindowManager::GetImplement();
        manager.draw();
    }

    void Interface::setClientRect(sint32 left, sint32 top, sint32 right, sint32 bottom)
    {
        impl::WindowManager& manager = impl::WindowManager::GetImplement();
        manager.setClientRect(Rect(left, top, right, bottom));
    }

} } // namespace nyanco::gui
