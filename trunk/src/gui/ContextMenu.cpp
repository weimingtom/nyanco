/*!
    @file   ContextMenu.cpp
    @author dasyprocta
 */

#include "ContextMenu.h"
#include "Graphics.hpp"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
MenuItemPtr MenuItem::Create(
    ComponentId const&              componentId,
    std::string const&              text)
{
    MenuItem* item = new MenuItem;

    item->m_id = componentId;
    item->text_ = text;
    item->setLocation(Rect(0, 0, 0, 14));

    return MenuItemPtr(item);
}

// ----------------------------------------------------------------------------
void MenuItem::draw(Graphics& g)
{
    if (isFocused())
    {
        g.drawText(Point(location_.left+3, location_.top+1), text_, 0xffeeeeee);
    }
    else
    {
        g.drawText(Point(location_.left+3, location_.top+1), text_, 0xffeeeeee);
    }
}

// ----------------------------------------------------------------------------
int MenuItem::getTextWidth() const
{
    return text_.size() * 6;
}

// ----------------------------------------------------------------------------
void MenuItem::setWidth(int width)
{
    location_.right = location_.left + width;
}

// ----------------------------------------------------------------------------
SubMenuItemPtr SubMenuItem::Create(
    ComponentId const&              componentId,
    std::string const&              text)
{
    SubMenuItem* item = new SubMenuItem;

    item->m_id = componentId;
    item->text_ = text;
    item->setLocation(Rect(0, 0, 0, 14));

    return SubMenuItemPtr(item);
}

// ----------------------------------------------------------------------------
void SubMenuItem::draw(Graphics& g)
{
    g.drawText(Point(location_.left+3, location_.top+1), text_, 0xffeeeeee);
}

// ----------------------------------------------------------------------------
int SubMenuItem::getTextWidth() const
{
    return text_.size() * 6;
}

// ----------------------------------------------------------------------------
void SubMenuItem::setWidth(int width)
{
    location_.right = location_.left + width;
}

// ----------------------------------------------------------------------------
MenuSeparatorPtr MenuSeparator::Create()
{
    MenuSeparator* item = new MenuSeparator;

    item->setLocation(Rect(0, 0, 0, 4));

    return MenuSeparatorPtr(item);
}

// ----------------------------------------------------------------------------
void MenuSeparator::draw(Graphics& g)
{
    g.setColor(0xff222222);
    g.drawLine(Point(location_.left+2, location_.top), Point(location_.right-2, location_.top));

    g.setColor(0xff888888);
    g.drawLine(Point(location_.left+2, location_.top+1), Point(location_.right-2, location_.top+1));
}

// ----------------------------------------------------------------------------
void MenuSeparator::setWidth(int width)
{
    location_.right = location_.left + width;
}

// ----------------------------------------------------------------------------
void ContextMenu::addItem(MenuItemBasePtr menuItem)
{
    itemList_.push_back(menuItem);
}

// ----------------------------------------------------------------------------
void ContextMenu::draw(Graphics& graphics)
{
    if (!showed_) return;

    graphics.setColor(0xff444444);
    graphics.drawFillRect(location_);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.right, location_.top));
    graphics.drawLine(Point(location_.left, location_.top), Point(location_.left, location_.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(location_.right, location_.top+1), Point(location_.right, location_.bottom));
    graphics.drawLine(Point(location_.left, location_.bottom), Point(location_.right, location_.bottom));

    using boost::bind;
    using boost::ref;
    std::for_each(itemList_.begin(), itemList_.end(), bind(&Component::draw, _1, ref(graphics)));
}

// ----------------------------------------------------------------------------
void ContextMenu::update()
{
    relocateItems();
}

// ----------------------------------------------------------------------------
void ContextMenu::visible(int x, int y)
{
    showed_ = true;

    location_.left = location_.right = x;
    location_.top = location_.bottom = y;
}

// ----------------------------------------------------------------------------
void ContextMenu::invisible()
{
    showed_ = false;
}

// ----------------------------------------------------------------------------
void ContextMenu::relocateItems()
{
    int maxWidth  = 0;

    // Y ’l‚ðÄ”z’u‚·‚é
    int locationY = location_.top + 2;
    foreach (MenuItemBasePtr p, itemList_)
    {
        p->setY(locationY);
        int height = p->getHeight();
        locationY += height + 2;

        int width = p->getTextWidth();
        if (width > maxWidth) maxWidth = width;
    }
    location_.right  = location_.left + maxWidth + 16;
    location_.bottom = locationY;

    // •‚ð‘µ‚¦‚é
    foreach (MenuItemBasePtr p, itemList_)
    {
        p->setX(location_.left);
        p->setWidth(maxWidth + 12);
    }
}

}} // namespace nyanco::gui
