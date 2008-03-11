#pragma once

/*!
    @file   ContextMenu.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Component.h"
#include "Event.h"
#include <list>

BEGIN_NAMESPACE_NYANCO_GUI

class ContextMenu;

namespace impl { class WindowManager; }

class MenuItemBase : public Component
{
public:
    virtual int getTextWidth() const { return 0; }
    virtual void setWidth(int width) = 0;
};

typedef boost::shared_ptr<MenuItemBase> MenuItemBasePtr;

class MenuItem : public MenuItemBase
{
public:
    static boost::shared_ptr<MenuItem> Create(
        ComponentId const&          componentId,
        std::string const&          text);

private:
    virtual void draw(Graphics& graphics);
    virtual int getTextWidth() const;
    virtual void setWidth(int width);

    std::string                     text_;

    friend ContextMenu;
};
typedef boost::shared_ptr<MenuItem> MenuItemPtr;

class SubMenuItem : public MenuItemBase
{
public:
    static boost::shared_ptr<SubMenuItem> Create(
        ComponentId const&          componentId,
        std::string const&          text);

private:
    virtual void draw(Graphics& g);
    virtual int getTextWidth() const;
    virtual void setWidth(int width);

    std::string                     text_;

    std::list<MenuItemBasePtr>      itemList_;

    friend ContextMenu;
};
typedef boost::shared_ptr<SubMenuItem> SubMenuItemPtr;

class MenuSeparator : public MenuItemBase
{
public:
    static boost::shared_ptr<MenuSeparator> Create();

private:
    virtual void draw(Graphics& g);
    virtual void setWidth(int width);
};
typedef boost::shared_ptr<MenuSeparator> MenuSeparatorPtr;

class ContextMenu : public Component
{
public:
    void addItem(
        MenuItemBasePtr             menuItem);

protected:
    void draw(Graphics&             graphics);
    void update();

    void visible(int x, int y);
    void invisible();

private:
    void relocateItems();

private:
    std::list<MenuItemBasePtr>      itemList_;
    bool                            showed_;

    ContextMenu() : showed_(false) {}

    friend impl::WindowManager;
};
typedef boost::shared_ptr<ContextMenu> ContextMenuPtr;

END_NAMESPACE_NYANCO_GUI
