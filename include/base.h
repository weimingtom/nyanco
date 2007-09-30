#pragma once

/*!
    @file   base.h
    @author dasyprocta
 */

namespace nyanco
{

    typedef signed int                  sint32;
    typedef signed short                sint16;
    typedef signed char                 sint8;
    typedef unsigned int                uint32;
    typedef unsigned short              uint16;
    typedef unsigned char               uint8;
    typedef float                       float32;
    typedef double                      float64;

    /*!
        @brief  pointer wrapper class
     */
    template <typename Type_>
    class Handle
    {
    public:
        Handle();
        ~Handle();

    private:

        friend Type_;
    };

} // namespace nyanco

