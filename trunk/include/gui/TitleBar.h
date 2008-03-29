#pragma once

/*!
    @file   TitleBar.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Component.h"

BEGIN_NAMESPACE_NYANCO_GUI

template <typename Derived_> class Frame;

class TitleBar : public Component
{
public:
    NYANCO_GUI_COMPONENT_TYPEDEF(TitleBar);

    static boost::shared_ptr<TitleBar> Create(
        ComponentId                 id,
        std::string const&          caption,
        boost::shared_ptr< Frame<void> > owner);

    void setCaption(std::string const& caption);
    std::string const& getCaption() const;

protected:
    virtual void draw(Graphics& graphics);
    virtual int getHeight() const;

    virtual bool onMouseProcess(MouseCommand const& mouse);

private:
    std::string                     caption_;
    bool                            onMoving_;
    bool                            onClosing_;
    boost::shared_ptr< Frame<void> > owner_;
};

typedef boost::shared_ptr<TitleBar> TitleBarPtr;

END_NAMESPACE_NYANCO_GUI
