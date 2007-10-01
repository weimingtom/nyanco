#pragma once

/*!
    @file   GuiInterface.h
    @author dasyprocta
 */

namespace nyanco
{
    class Framework;

    class GuiInterface
    {
    public:
        typedef GuiInterface* (*Factory)();

        static void RegistFactory(Factory factory)
        {
            Factory_ = factory;
        }

        static GuiInterface* Create()
        {
            if (Factory_ != 0)
                return Factory_();
            return new GuiInterface; // null interface
        }

    protected:
        virtual void onUpdate() {}
        virtual void onDraw() {}

        static Factory                  Factory_;

        friend Framework;
    };

} // namespace nyanco
