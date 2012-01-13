// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2012 Mateusz Loskot, London, UK.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_GIS_GEOMETRIES_OGC_HPP
#define BOOST_GEOMETRY_EXTENSIONS_GIS_GEOMETRIES_OGC_HPP

#include <boost/cstdint.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>
#include <boost/variant.hpp>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace boost { namespace geometry
{

namespace model { namespace ogc
{

// enum wkbGeometryType
// {
//   wkbPoint = 1,
//   wkbLineString = 2,
//   wkbPolygon = 3,
//   wkbMultiPoint = 4,
//   wkbMultiLineString = 5,
//   wkbMultiPolygon = 6,
//   wkbGeometryCollection = 7
// };

BOOST_SCOPED_ENUM_START(wkb_type)
{
    unknown         = 0, // boost::geometry specific
    point           = 1,
    linestring      = 2,
    polygon         = 3,
    // TODO: Not implemented
    multipoint      = 4,
    multilinestring = 5,
    multipolygon    = 6,
    collection      = 7,
};
BOOST_SCOPED_ENUM_END

BOOST_SCOPED_ENUM(model::ogc::wkb_type) make_wkb_type(boost::uint32_t id)
{
    using model::ogc::wkb_type;
    BOOST_SCOPED_ENUM(wkb_type) type(wkb_type::unknown);

    switch (id)
    {
    case 1:
        type = wkb_type::point;
        break;
    case 2:
        type = wkb_type::linestring;
        break;
    case 3:
        type = wkb_type::polygon;
        break;
    case 4:
        type = wkb_type::multipoint;
        break;
    case 5:
        type = wkb_type::multilinestring;
        break;
    case 6:
        type = wkb_type::multipolygon;
        break;
    case 7:
        type = wkb_type::collection;
        break;
    }
    return type;
}

typedef model::d2::point_xy<double> point;
typedef model::linestring<point> linestring;
typedef model::polygon<point, true, true> polygon;
typedef boost::variant<point, linestring, polygon> any_geometry;

#if 0
template <typename T>
struct ogc_geometry : public boost::variant
    <
        model::d2::point_xy<T>
        /*model::linestring<model::d2::point_xy<T>>,
        model::polygon<model::d2::point_xy<T>, true, true>*/
        //model::d2::polygon<model::d2::point_xy<T>, false, true>,
    >
{
    typedef T coodinate_type;
    typedef model::d2::point_xy<T> point_type;
    typedef model::linestring<point_type> linestring_type;
    typedef model::polygon<point_type, true, true> polygon_type;

    //og
};
#endif

}} // namespace model::ogc

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_EXTENSIONS_GIS_GEOMETRIES_OGC_HPP
