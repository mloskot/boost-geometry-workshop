// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_READ_WKB_HPP
#define BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_READ_WKB_HPP

#include <iterator>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/static_assert.hpp>

#include <boost/geometry/core/tags.hpp>
#include <boost/geometry/extensions/gis/io/wkb2/detail/parser.hpp>

namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename Tag, typename G>
struct read_wkb {};

template <typename G>
struct read_wkb<point_tag, G>
{
    template <typename Iterator>
    static inline bool parse(Iterator& it, Iterator end, G& geometry,
        BOOST_SCOPED_ENUM(detail::wkb::byte_order) order)
    {
        return detail::wkb::point_parser<G>::parse(it, end, geometry, order);
    }
};

template <typename G>
struct read_wkb<linestring_tag, G>
{
    template <typename Iterator>
    static inline bool parse(Iterator& it, Iterator end, G& geometry,
        BOOST_SCOPED_ENUM(detail::wkb::byte_order) order)
    {
        geometry::clear(geometry);
        return detail::wkb::linestring_parser<G>::parse(it, end, geometry, order);
    }
};

template <typename G>
struct read_wkb<polygon_tag, G>
{
    template <typename Iterator>
    static inline bool parse(Iterator& it, Iterator end, G& geometry,
        BOOST_SCOPED_ENUM(detail::wkb::byte_order) order)
    {
        geometry::clear(geometry);
        return detail::wkb::polygon_parser<G>::parse(it, end, geometry, order);
    }
};

template <typename G>
struct parser
{
    template <typename Iterator>
    static inline bool apply(Iterator& it, Iterator end, G& geometry,
        BOOST_SCOPED_ENUM(detail::wkb::byte_order) order)

    {
        return dispatch::read_wkb
            <
            typename tag<G>::type,
            G
            >::parse(begin, end, geometry, byte_order);
    }
};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


template <typename Iterator, typename G>
inline bool read_wkb(Iterator begin, Iterator end, G& geometry)
{
    // Stream of bytes can only be parsed using random access iterator.
    BOOST_STATIC_ASSERT((
        boost::is_convertible
        <
            typename std::iterator_traits<Iterator>::iterator_category,
            const std::random_access_iterator_tag&
        >::value));

    BOOST_SCOPED_ENUM(detail::wkb::byte_order) order;
    if (detail::wkb::byte_order_parser::parse(begin, end, order))
    {
        return dispatch::read_wkb
            <
            typename tag<G>::type,
            G
            >::parse(begin, end, geometry, byte_order);
    }

    return false;
}

template <typename ByteType, typename G>
inline bool read_wkb(ByteType const* bytes, std::size_t length, G& geometry)
{
    BOOST_STATIC_ASSERT((boost::is_integral<ByteType>::value));
    BOOST_STATIC_ASSERT((sizeof(boost::uint8_t) == sizeof(ByteType)));

    ByteType const* begin = bytes;
    ByteType const* const end = bytes + length;

    return read_wkb(begin, end, geometry);
}

template <typename Iterator>
inline bool read_wkb(Iterator begin, Iterator end, model::ogc::any_geometry& out)
{
    namespace bgwkb = detail::wkb;

    BOOST_SCOPED_ENUM(detail::wkb::byte_order) order;
    if (!bgwkb::byte_order_parser::parse(begin, end, order))
        return false;

    BOOST_SCOPED_ENUM(model::ogc::wkb_type) kind;
    if (!bgwkb::geometry_type_parser::parse(begin, end, kind, order))
        return false;

#if 0
    typedef ogc_geometry<CoordinateType> ogc_type;
    if (bgwkb::geometry_type::point == kind)
    {
        typedef model::d2::point_xy<CoordinateType> output_type;
        output_type g;
        if (dispatch::parser<output_type>::apply(begin, end, g, order))
        {
            //out = g;
            return true;
        }
    }
    else if (bgwkb::geometry_type::linestring == kind)
    {
        typedef model::linestring<point_type> output_type;
        output_type g;

        if (dispatch::parser<output_type>::apply(begin, end, g, order))
        {
            //out = g;
            return true;
        }
    }
    else if (bgwkb::geometry_type::polygon == kind)
    {
        linestring_type;
    typedef model::polygon<point_type, true, true> polygon_type;
    
        typedef ogc_type::polygon_type output_type;
        output_type g;
        if (dispatch::parser<output_type>::apply(begin, end, g, order))
        {
            //out = g;
            return true;
        }
    }
    else
    {
        ; // TODO: multi
    }
#endif

    return false;
}

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_EXTENSIONS_GIS_IO_WKB_READ_WKB_HPP
