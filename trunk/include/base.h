#pragma once

/*!
    @file   base.h
    @author dasyprocta
 */

#include <boost/cstdint.hpp>

#define BEGIN_NAMESPACE_NYANCO  namespace nyanco {
#define END_NAMESPACE_NYANCO    }
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

END_NAMESPACE_NYANCO
