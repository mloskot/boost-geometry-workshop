#ifndef BOOST_GEOMETRY_WORKSHOP_GEOMETRY_UTILITY_HPP
#define BOOST_GEOMETRY_WORKSHOP_GEOMETRY_UTILITY_HPP

namespace workshop {

inline std::vector<uint8_t> make_wkb(std::string hex)
{
    std::vector<unsigned char> data;
    boost::geometry::hex2wkb(hex, std::back_inserter(data));
    return data;
}

} // namespace workshop

#endif