#pragma once

/*!
    @file   WindowManager.hpp
    @author dasyprocta
 */

#include "gui/WindowManager.h"
#include "Graphics.hpp"
#include <d3d9.h>
#include <list>

namespace nyanco { namespace gui
{
    class Frame;

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
            nyanco::gui::Frame*         framePtr);

        virtual void detach(
            Frame*                      framePtr);

        virtual void detach(
            std::string const&          name);

        virtual nyanco::gui::Frame* search(
            std::string const&          name);

        virtual Frame* getActiveWindow() const;

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
        std::list<Frame*>               frameList_;
        Graphics*                       graphics_;

        static WindowManager*           myPtr_;

        friend Interface;
        friend nyanco::gui::WindowManager;
    };

} } } // namespace nyanco::gui::impl