/*!
    @file   ListBoxTest.cpp
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
        ListBoxId,
        AddButtonId,
        DeleteButtonId,
    };

    ListBox::Ptr                        m_listBox;

public:
    MyFrame()
    {
        ListBox::Ptr listBox = ListBox::Create(ListBoxId, ListBox::Arg().height(240));
        listBox->addColumn("Column1", 100).addColumn("Column2", 100);
        for (int i = 0; i < 20; ++i)
        {
            listBox->addRow();
            for (int j = 0; j < 2; ++j)
            {
                std::string str = "test " + boost::lexical_cast<std::string>(i) + " " + boost::lexical_cast<std::string>(j);
                listBox->getItem(i, j).set(str);
            }
        }

        attach(listBox);
        attach(Button::Create(AddButtonId, "Add"));
        attach(Button::Create(DeleteButtonId, "Delete"));

        registerHandler(AddButtonId, &MyFrame::onAddButtonEvent, this);
        registerHandler(DeleteButtonId, &MyFrame::onDeleteButtonEvent, this);

        m_listBox = listBox;
    }

    void onAddButtonEvent(Event<Button> const& e)
    {
        m_listBox->addRow();
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
        manager.attach(MyFrame::Create(-1, Frame<>::Arg().caption("Test ListBox").width(280)));
    }
};

NYA_REGIST_APP(MyApplication)
