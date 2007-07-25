#pragma once

/*!
    @file   Application.hpp
    @author dasyprocta
 */

namespace nyanco
{

class Framework;

class Application
{
protected:
    // --------------------------------------------------------------------
    /*!

     */
    // --------------------------------------------------------------------
    virtual void onInitialize() {}

    // --------------------------------------------------------------------
    /*!

     */
    // --------------------------------------------------------------------
    virtual void onFinalize() {}

    // --------------------------------------------------------------------
    /*!

     */
    // --------------------------------------------------------------------
    virtual void onUpdate() {}

    // --------------------------------------------------------------------
    /*!

     */
    // --------------------------------------------------------------------
    virtual void onDraw() {}

    virtual void onResetDevice() {}
    virtual void onLostDevice() {}

    friend class Framework;
};

} // namespace nyanco

#define NYA_REGIST_APP(app_)                \
    nyanco::Application* NyancoCreateApp()  \
    {                                       \
        return new app_;                    \
    }                                       \

