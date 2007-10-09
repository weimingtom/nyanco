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

        static bool RegistFactory(Factory factory)
        {
            Factory_ = factory;
            return true;
        }

        static GuiInterface* Instanciate()
        {
            if (Factory_ != 0)
                return Factory_();
            return new GuiInterface; // null interface
        }

    protected:
        virtual void onInitialize() {}
        virtual void onFinalize()   {}
        virtual void onUpdate()     {}
        virtual void onDraw()       {}

        static  void Implement();
        static Factory                  Factory_;

        friend Framework;
    };

} // namespace nyanco
