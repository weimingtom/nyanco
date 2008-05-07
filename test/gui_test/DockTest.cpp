/*!
    @file   DockTest.cpp
    @author dasyprocta
 */

#include "nyanco.h"
#include "gui/gui.h"

using namespace nyanco;
using namespace nyanco::gui;

// ============================================================================
class MyFrame : public Frame<MyFrame>
{
    enum
    {
        DockLeftButtonId,
        DockRightButtonId,
        DockBottomButtonId,
        DockTopButtonId,
    };

public:
    MyFrame()
    {
        SplitPanel<2>::Ptr splitPanel = SplitPanel<2>::Create(-1);
        splitPanel->get<0>()->attach(Button::Create(DockLeftButtonId, "Dock Left"));
        splitPanel->get<0>()->attach(Button::Create(DockTopButtonId, "Dock Top"));
        splitPanel->get<1>()->attach(Button::Create(DockRightButtonId, "Dock Right"));
        splitPanel->get<1>()->attach(Button::Create(DockBottomButtonId, "Dock Buttom"));
        attach(splitPanel);

        registerHandler(DockLeftButtonId, &MyFrame::onPushButton, this);
        registerHandler(DockRightButtonId, &MyFrame::onPushButton, this);
        registerHandler(DockTopButtonId, &MyFrame::onPushButton, this);
        registerHandler(DockBottomButtonId, &MyFrame::onPushButton, this);
    }

    void onPushButton(Event<Button> const& e)
    {
        WindowManager& manager = WindowManager::GetInterface();
        Frame<>::Ptr this_ = manager.search(-1);
        if (this_->isDocked()) manager.undock(this_);
        else
        {
            switch (e.getSenderId())
            {
            case DockLeftButtonId:
                manager.dock(this_, Dock::Left);
            case DockRightButtonId:
                manager.dock(this_, Dock::Right);
            case DockBottomButtonId:
                manager.dock(this_, Dock::Bottom);
            case DockTopButtonId:
                manager.dock(this_, Dock::Top);
                break;
            }
        }
    }
};

// ============================================================================
class MyApplication : public Application
{
    void onInitialize()
    {
        WindowManager& manager = WindowManager::GetInterface();
        manager.attach(MyFrame::Create(-1, Frame<>::Arg().caption("Test Dock").width(480)));
    }
};

NYA_REGIST_APP(MyApplication)
