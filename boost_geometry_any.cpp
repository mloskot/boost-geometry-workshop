#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/extensions/gis/io/wkb/read_wkb.hpp>
#include <boost/geometry/extensions/gis/io/wkb/utility.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <boost/variant.hpp>
#include "geometry_utility.hpp"
using namespace std;
using namespace workshop;

namespace boost { namespace geometry {

template <typename T>
struct any_geometry : public boost::variant<model::d2::point_xy<T>>
{
};

}} // namespace boost::geometry

namespace bg = boost::geometry;

int main()
{
    try
    {
        bg::any_geometry<double> a1;

        auto wkb = make_wkb("0101000000000000000000f03f0000000000000040");
        if (!bg::read_wkb(wkb.cbegin(), wkb.cend(), a1))
            throw std::runtime_error("read_wkb failed");

        //clog << bg::wkt(p0) << std::endl;
    }
    catch (std::exception const& e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}