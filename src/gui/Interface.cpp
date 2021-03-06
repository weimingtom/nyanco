/*!
    @file   Interface.cpp
    @author dasyprocta
 */

#include "gui_base.h"
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
        manager.setClientRect(Rect<sint32>(left, top, right, bottom));
    }

    void Interface::getClientRect(sint32& left, sint32& top, sint32& right, sint32& bottom)
    {
        impl::WindowManager& manager = impl::WindowManager::GetImplement();
        Rect<sint32> const& rect = manager.getViewRect();
        left    = rect.left;
        top     = rect.top;
        right   = rect.right;
        bottom  = rect.bottom;
    }

} } // namespace nyanco::gui
