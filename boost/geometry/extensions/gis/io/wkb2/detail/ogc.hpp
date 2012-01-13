// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_DETAIL_OGC_HPP
#define BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_DETAIL_OGC_HPP

#include <boost/cstdint.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>

namespace boost { namespace geometry
{

// The well-known binary representation for OGC geometry (WKBGeometry),
// provides a portable representation of a geometry value as a contiguous
// stream of bytes. It permits geometry values to be exchanged between
// a client application and an SQL database in binary form.
//
// Basic Type definitions
// byte : 1 byte
// uint32 : 32 bit unsigned integer (4 bytes)
// double : double precision number (8 bytes)
//
// enum wkbByteOrder
// {
//   wkbXDR = 0, // Big Endian
//   wkbNDR = 1  // Little Endian
// };

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace wkb
{

BOOST_SCOPED_ENUM_START(byte_order)
{
    xdr     = 0, // wkbXDR, bit-endian
    ndr     = 1, // wkbNDR, little-endian
    unknown = 2  // not defined by OGC
};
BOOST_SCOPED_ENUM_END

}} // namespace detail::wkb
#endif // DOXYGEN_NO_IMPL

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_DETAIL_OGC_HPP
