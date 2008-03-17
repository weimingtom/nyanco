#pragma once

/*!
    @file   TitleBar.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Component.h"
#include "Frame.h"

BEGIN_NAMESPACE_NYANCO_GUI

class TitleBar : public Component
{
public:
    static boost::shared_ptr<TitleBar> Create(
        ComponentId                 id,
        std::string const&          caption,
        FramePtr                    owner);

    void setCaption(std::string const& caption);
    std::string const& getCaption() const;

protected:
    virtual void draw(Graphics& graphics);
    virtual void update();
    virtual int getHeight() const;

    virtual bool onMouseProcess(MouseCommand const& mouse);

private:
    std::string                     caption_;
    bool                            onMoving_;
    bool                            onClosing_;
    FramePtr                        owner_;
};

typedef boost::shared_ptr<TitleBar> TitleBarPtr;

END_NAMESPACE_NYANCO_GUI
