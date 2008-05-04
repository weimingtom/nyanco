#pragma once

/*!
    @file   WindowManager.hpp
    @author dasyprocta
 */

#include "gui_impl_base.hpp"
#include "gui/WindowManager.h"
#include "Graphics.hpp"
#include "Component.h"
#include <d3d9.h>
#include <vector>
#include <list>
#include <boost/scoped_ptr.hpp>

BEGIN_NAMESPACE_NYANCO

class Mouse;
class Keyboard;

END_NAMESPACE_NYANCO
BEGIN_NAMESPACE_NYANCO_GUI

template <typename Type_> class Frame;
class ContextMenu;
class Interface;

END_NAMESPACE_NYANCO_GUI
BEGIN_NAMESPACE_NYANCO_GUI_IMPL

class WindowManager : public nyanco::gui::WindowManager
{
    virtual void drawText(
        Point const&                point,
        std::string const&          text,
        Color                       color);

    virtual void attach(
        nyanco::gui::Frame<>::Ptr       framePtr);

    virtual void detach(
        Frame<>::Ptr                framePtr);

    virtual void detach(
        ComponentId                 id);

    virtual Frame<>::Ptr search(
        ComponentId                 id);

    virtual void activate(
        Frame<>::Ptr                framePtr);

    virtual Frame<>::Ptr getActiveWindow() const;

    virtual ContextMenuPtr getContextMenu() const;

    void initialize(
        LPDIRECT3DDEVICE9           devicePtr);

    void finalize();

    void update();

    void draw();

    static WindowManager& GetImplement();

    
    //void onMouseProcess(Mouse const& mouse);
    //void onKeyboardProcess(Keyboard const& keyboard);
    void onInputProcess(Keyboard const& keyboard, Mouse const& mouse);

private:
    class InputState
    {
    private:
        Point                       clickPoint_;
    };

    class Text
    {
    public:
        Text(
            Point const&            point,
            std::string const&      text,
            Color                   color)
            : point_(point), text_(text), color_(color) {}

        void draw(Graphics& graphics)
        {
//            graphics.drawText(point_, text_, color_);
        }

    private:
        Point                       point_;
        std::string                 text_;
        Color                       color_;
    };

    typedef std::list<Frame<>::Ptr> FramePtrList;
    typedef std::vector<Text>       TextList;

    //! フレームリスト
    FramePtrList                    framePtrList_;
    //! テキスト
    TextList                        textList_;
    //! メニュー
    ContextMenuPtr                  contextMenu_;

    Component::WeakPtr              m_capturedMouse;
    Component::WeakPtr              m_capturedKeyboard;

    FramePtrList                    killedFramePtrList_;

    Rect                            m_clientRect;

    boost::scoped_ptr<Graphics>     graphics_;

    static WindowManager*           myPtr_;

    friend Interface;
    friend nyanco::gui::WindowManager;
};

END_NAMESPACE_NYANCO_GUI_IMPL
