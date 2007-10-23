#pragma once

/*!
    @file   Component.h
    @author dasyprocta
 */

#include "gui_base.h"
#include <string>
#include <boost/enable_shared_from_this.hpp>

namespace nyanco { namespace gui
{
    class Graphics;
    class Container;
    class Frame;

    namespace impl { class WindowManager; }

    class Component : public boost::enable_shared_from_this<Component>
    {
    public:
        std::string const& getName() const { return name_; }
        void focus();
        bool isFocused();

    protected:
        virtual void draw(Graphics& graphics) = 0;
        virtual int getHeight() const;
        virtual void resize(int parentWidth);
        virtual boost::shared_ptr<Component> checkHit(int x, int y);
        virtual void update();
        virtual void move(int x, int y);

        virtual void onMouseProcess(MouseCommand const& mouse) {}
        virtual void onKeyboardPtrocess(KeyboardCommand const& keyboard) {}

        bool isPointInner(Point const& point);
        void setName(std::string const& name);
        void setLocation(Rect const& location);
        //void setPosition(int x, int y);
        void setX(int x);
        void setY(int y);
        void setWidth(int width);

    protected:
        std::string                     name_;
        Rect                            location_;

        friend Frame;
        friend Container;
        friend impl::WindowManager;
    };

    typedef boost::shared_ptr<Component> ComponentPtr;

} } // namespace nyanco::gui
