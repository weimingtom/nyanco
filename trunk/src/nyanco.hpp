#pragma once

/*!
    @file   nyanco.hpp
    @author dasyprocta
 */

namespace nyanco
{
    template <typename T_>
    inline bool isNull(T_* const ptr)
    {
        return ptr == 0;
    }

    template <typename T_>
    inline bool isValid(T_* const ptr)
    {
        return ptr != 0;
    }

    template <typename T_>
    void safeRelease(T_*& ptr)
    {
        if (isValid(ptr))
        {
            ptr->Release();
            ptr = 0;
        }
    }

    template <typename T_>
    void safeDelete(T_*& ptr)
    {
        if (isValid(ptr))
        {
            delete ptr;
            ptr = 0;
        }
    }

    template <typename T_>
    void safeDeleteArray(T_*& ptr)
    {
        if (isValid(ptr))
        {
            delete[] ptr;
            ptr = 0;
        }
    }
}
