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

    NYANCO_GUI_ARG_DEF_MIXIN2_BEGIN(LabeledTextEdit, TextEdit, Label)
        // TODO: ラベルとエディットの画面割合とかどっちのサイズを固定するとか
    NYANCO_GUI_ARG_DEF_END(LabeledTextEdit)

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
    TextEdit::Ptr                      m_textEdit;
};

END_NAMESPACE_NYANCO_GUI
