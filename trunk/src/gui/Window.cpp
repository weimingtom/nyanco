/*!
    @file   Window.cpp
    @author dasyprocta
 */

#include "Window.h"
#include "Container.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
void Window::moveTo(int x, int y)
{
    m_location.right     = x + m_location.getWidth();
    m_location.bottom    = y + m_location.getHeight();
    m_location.left      = x;
    m_location.top       = y;
}

// ----------------------------------------------------------------------------
void Window::move(int x, int y)
{
    m_location.left  += x;
    m_location.right += x;
    m_location.top   += y;
    m_location.bottom += y;
}

// ----------------------------------------------------------------------------
void Window::focus(Component::Ptr component)
{
    if (Component::Ptr comp = m_focusedComponent.lock()) comp->defocus();
    m_focusedComponent = component;
    if (Component::Ptr comp = m_focusedComponent.lock()) comp->focus();
}

// ----------------------------------------------------------------------------
void Window::defocus()
{
    if (Component::Ptr comp = m_focusedComponent.lock())
    {
        m_prevFocused = comp;
        comp->defocus();
    }
    m_focusedComponent.reset();
}

// ----------------------------------------------------------------------------
void Window::revertFocus()
{
    if (Component::Ptr comp = m_prevFocused.lock())
    {
        m_focusedComponent = comp;
        m_prevFocused.reset();
    }
}

// ----------------------------------------------------------------------------
Component::Ptr Window::getFocusedComponent() const
{
    return m_focusedComponent.lock();
}

// ----------------------------------------------------------------------------
bool Window::isPointInner(Point const& point) const
{
    return (m_location.left <= point.x && point.x < m_location.right &&
            m_location.top  <= point.y && point.y < m_location.bottom);
}

END_NAMESPACE_NYANCO_GUI
