#pragma once

/*!
    @file   Label.h
    @author dasyprocta
 */

#include "Component.h"

namespace nyanco { namespace gui
{

    class Label : public Component
    {
    public:
        static boost::shared_ptr<Label> Create(
            std::string const&          text);

    private:
        virtual void draw(Graphics& graphics);
        virtual int getHeight() const;

        std::string                     text_;
    };

    typedef boost::shared_ptr<Label> LabelPtr;

} } // namespace nyanco::gui
