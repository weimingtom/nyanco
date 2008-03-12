#pragma once

/*!
    @file   Frame.h
    @author dasyprocta
 */

#include "gui_base.h"
#include "Container.h"

BEGIN_NAMESPACE_NYANCO_GUI

class WindowManager;
class Graphics;

namespace impl { class WindowManager; }

class Frame : public Container
{
public:
    static boost::shared_ptr<Frame> Create(
        std::string const&          name,
        std::string const&          caption,
        uint32                      width,
        uint32                      height);

    void moveTo(int x, int y);
    void move(int x, int y);

protected:
    void draw(Graphics& graphics);
    void update();

private:
    void relocateY();
    ComponentPtr getHitComponent(int x, int y);

private:
    ComponentPtr                    focusedComponent_;
    std::string                     caption_;

    friend impl::WindowManager;
};

typedef boost::shared_ptr<Frame> FramePtr;

END_NAMESPACE_NYANCO_GUI
