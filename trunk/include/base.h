#pragma once

/*!
    @file   base.h
    @author dasyprocta
 */

#pragma warning(disable: 4996)
#pragma warning(disable: 4819)

#include <boost/cstdint.hpp>
#include <windows.h>

#define BEGIN_NAMESPACE_NYANCO  namespace nyanco {
#define END_NAMESPACE_NYANCO    }
#define BEGIN_NO_NAMESPACE      namespace {
#define END_NO_NAMESPACE        }

#define USING_NAMESPACE_NYANCO  using namespace nyanco;

BEGIN_NAMESPACE_NYANCO

typedef boost::int32_t                  sint32;
typedef boost::int16_t                  sint16;
typedef boost::int8_t                   sint8;
typedef boost::uint32_t                 uint32;
typedef boost::uint16_t                 uint16;
typedef boost::uint8_t                  uint8;
typedef float                           float32;
typedef double                          float64;

typedef sint8                           char8;
typedef wchar_t                         wchar;

END_NAMESPACE_NYANCO

#ifdef max
#   undef max
#endif
#ifdef min
#   undef min
#endif
