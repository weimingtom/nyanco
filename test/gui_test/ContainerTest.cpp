/*!
    @file   ContainerTest.cpp
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
        Group1Id,
        Group2Id,
    };

public:
    MyFrame()
    {
        Group::Ptr group1 = Group::Create(Group1Id, "Group1");
        group1->attach(Button::Create(-1, "Button 1"));
        group1->attach(Button::Create(-1, "Button 2"));
        Group::Ptr group2 = Group::Create(Group2Id, "Group2");
        group1->attach(group2);
        group2->attach(Button::Create(-1, "Button 3"));
        group2->attach(Button::Create(-1, "Button 4"));
        group1->attach(Button::Create(-1, "Button 5"));

//        attach(group1);

        SplitPanel<2>::Ptr splitPanel = SplitPanel<2>::Create(-1);
        splitPanel->get<0>()->attach(group1);
        splitPanel->get<1>()->attach(Button::Create(-1, "Button 6"));

        attach(splitPanel);
    }
};

// ============================================================================
class MyApplication : public Application
{
    void onInitialize()
    {
        WindowManager& manager = WindowManager::GetInterface();
        manager.attach(MyFrame::Create(-1, Frame<>::Arg().caption("Test Container").width(480)));
    }
};

NYA_REGIST_APP(MyApplication)
