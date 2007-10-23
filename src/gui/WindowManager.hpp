#pragma once

/*!
    @file   WindowManager.hpp
    @author dasyprocta
 */

#include "gui/WindowManager.h"
#include "Graphics.hpp"
#include <d3d9.h>
#include <vector>
#include <list>

namespace nyanco
{
    class Mouse;
    class Keyboard;

namespace gui
{
    class Frame;
    class Interface;

namespace impl
{
    class WindowManager : public nyanco::gui::WindowManager
    {
    public:
        virtual void drawText(
            Point const&                point,
            std::string const&          text,
            Color                       color);
        
        virtual void attach(
            nyanco::gui::FramePtr       framePtr);

        virtual void detach(
            FramePtr                    framePtr);

        virtual void detach(
            std::string const&          name);

        virtual FramePtr search(
            std::string const&          name);

        virtual void activate(
            FramePtr                    framePtr);

        virtual FramePtr getActiveWindow() const;

        void initialize(
            LPDIRECT3DDEVICE9           devicePtr);

        void finalize();

        void update();

        void draw();

        static WindowManager& GetImplement();

        void onMouseProcess(Mouse const& mouse);
        void onKeyboardProcess(Keyboard const& keyboard);

        void onKeyPush();
        void onKeyUp();

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
                graphics.drawText(point_, text_, color_);
            }

        private:
            Point                       point_;
            std::string                 text_;
            Color                       color_;
        };

        typedef std::list<FramePtr>     FramePtrList;
        typedef std::vector<Text>       TextList;

        FramePtrList                    framePtrList_;
        FramePtrList                    killedFramePtrList_;
        TextList                        textList_;

        Graphics*                       graphics_;

        static WindowManager*           myPtr_;

        friend Interface;
        friend nyanco::gui::WindowManager;
    };

} } } // namespace nyanco::gui::impl