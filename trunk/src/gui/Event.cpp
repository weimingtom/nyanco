/*!
    @file   Event.cpp
    @author dasyprocta
 */

#include "Event.h"
#include <iostream>

// test
BEGIN_NO_NAMESPACE

void testEventServer()
{
    using namespace nyanco::gui;

    struct Button {};
    struct List {};
    struct CheckBox {};

    class Frame : public EventServer
    {
        void onClickButton(Event<Button> const& e)
        {
            std::cout << "hello button!!\n";
        }

        void onClickList(Event<List> const& e)
        {
            std::cout << "hello list!!\n";
        }

    public:
        Frame()
        {
            ;
            registerHandler(ButtonId, &Frame::onClickButton);
            registerHandler(ListId, &Frame::onClickList);
        }

        enum ComponentId
        {
            ButtonId,
            ListId,
            CheckBoxId,
        };
    };

    EventServer es;
    es.queueEvent(Frame::ButtonId, Event<Button>());
    es.invokeHandler();
}

END_NO_NAMESPACE
