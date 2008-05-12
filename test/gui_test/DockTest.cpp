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
        CreateButtonId,
    };

public:
    MyFrame()
    {
        SplitPanel<2>::Ptr splitPanel = SplitPanel<2>::Create(-1);
        splitPanel->get<0>()->attach(Button::Create(DockLeftButtonId, "Dock Left"));
        splitPanel->get<0>()->attach(Button::Create(DockTopButtonId, "Dock Top"));
        splitPanel->get<1>()->attach(Button::Create(DockRightButtonId, "Dock Right"));
        splitPanel->get<1>()->attach(Button::Create(DockBottomButtonId, "Dock Bottom"));
        attach(splitPanel);
        attach(Button::Create(CreateButtonId, "Create"));

        registerHandler(DockLeftButtonId, &MyFrame::onPushButton);
        registerHandler(DockRightButtonId, &MyFrame::onPushButton);
        registerHandler(DockTopButtonId, &MyFrame::onPushButton);
        registerHandler(DockBottomButtonId, &MyFrame::onPushButton);
        registerHandler(CreateButtonId, &MyFrame::onPushCreate);
    }

    void onPushButton(Event<Button> const& e)
    {
        WindowManager& manager = WindowManager::GetInterface();
        Frame<>::Ptr this_ = boost::shared_static_cast< Frame<> >(shared_from_this());
        if (this_->isDocked()) manager.undock(this_);
        else
        {
            switch (e.getSenderId())
            {
            case DockLeftButtonId:
                manager.dock(this_, Dock::Left);
                break;
            case DockRightButtonId:
                manager.dock(this_, Dock::Right);
                break;
            case DockBottomButtonId:
                manager.dock(this_, Dock::Bottom);
                break;
            case DockTopButtonId:
                manager.dock(this_, Dock::Top);
                break;
            }
        }
    }

    void onPushCreate(Event<Button> const& e)
    {
        WindowManager& manager = WindowManager::GetInterface();
        manager.attach(MyFrame::Create(-1, Frame<>::Arg().caption("Test Dock").width(480)));
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
