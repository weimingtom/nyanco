#pragma once

/*!
    @file   LabeledEdit.h
    @author dasyprocta
 */

#include "Label.h"
#include "EditField.h"
#include "SplitPanel.h"
#include "Event.h"

BEGIN_NAMESPACE_NYANCO_GUI

class LabeledTextEdit : public Component, public EventServer
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(LabeledTextEdit);

    template <typename Class_ = LabeledTextEdit>
    struct Arg : public TextField::Arg, public Label::Arg<Class_>
    {
        // TODO: ラベルとエディットの画面割合とかどっちのサイズを固定するとか
    };

    static Ptr Create(Arg<> const& arg, ComponentId id = NonspecificId);

private:
    void update();
    void draw(Graphics& graphics);
    int getHeight() const;
    bool onMouseProcess(MouseCommand const& command);
    bool onKeyboardProcess(KeyboardCommand const& command);
    sint32 relocate(sint32 parentLeft, sint32 parentWidth, sint32 locationY);
    void move(int x, int y);

    void focus();
    void defocus();

    LabeledTextEdit() : m_splitPanel(SplitPanel<2>::Create(-1)) {}

    SplitPanel<2>::Ptr                  m_splitPanel;
    Label::Ptr                          m_label;
    TextField::Ptr                      m_textField;
};

END_NAMESPACE_NYANCO_GUI
