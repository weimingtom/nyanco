/*!
    @file   EditTest.cpp
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
        TextEditId
    };

public:
    MyFrame()
    {
        attach(TextEdit::Create(TextEditId));
    }

    void onAddButtonEvent(Event<Button> const& e)
    {

    }

    void onDeleteButtonEvent(Event<Button> const& e)
    {

    }
};

// ============================================================================
class MyApplication : public Application
{
    void onInitialize()
    {
        WindowManager& manager = WindowManager::GetInterface();
        manager.attach(MyFrame::Create(-1, Frame<>::Arg().caption("Test Edit").width(280)));
    }
};

NYA_REGIST_APP(MyApplication)
