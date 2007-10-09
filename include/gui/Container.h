#pragma once

/*!
    @file   Container.h
    @author dasyprocta
 */

#include "Component.h"
#include <list>

namespace nyanco { namespace gui
{

    class Container : public Component
    {
    public:
        virtual void attach(
            Component*                  componentPtr);

        virtual void detach(
            Component*                  componentPtr);

        virtual void detach(
            std::string const&          componentName);

    protected:
        typedef std::list<Component*>   ComponentList;
        ComponentList                   componentList_;
    };

} } // namespace nyanco::gui
