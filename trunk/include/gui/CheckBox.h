#pragma once

/*!
    @file   CheckBox.h
    @author dasyprocta
 */

#include "Component.h"

namespace nyanco { namespace gui
{

    class CheckBox : public Component
    {
    public:
        static boost::shared_ptr<CheckBox> Create(
            std::string const&          name,
            std::string const&          text,
            bool                        check = false);

        void setCheck(bool check);
        bool getCheck() const;

    private:
        virtual void draw(Graphics& graphics);
        virtual int getHeight() const;

        std::string                     text_;
        bool                            check_;
    };

    typedef boost::shared_ptr<CheckBox> CheckBoxPtr;

}} // namespace nyanco::gui
