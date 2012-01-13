// Kind of "minimal" reproduction to verify if MSVC hosting namespace does work.
// See the line with _MSC_VER

#include <iostream>

namespace boost
{

// Mockup true/false
struct true_type 
{
    typedef true_type type;
    static bool const value = true;
};

struct false_type 
{
    typedef false_type type;
    static bool const value = false;
};

}

namespace boost { namespace geometry
{

// Mockup basic Boost.Geometry tags

struct point_tag {};
struct ring_tag {};
struct cartesian_tag {};

// Mockup "tag"
namespace traits
{
template <typename Geometry> struct tag {};
}
template <typename Geometry> struct tag 
{
    typedef typename traits::tag<Geometry>::type type;
};

// Mockup "point_type"
namespace traits
{
template <typename Geometry> struct point_type {};
}


template <typename Geometry>
struct point_type
{
    // Default: call traits to get point type
    typedef typename traits::point_type<Geometry>::type type;
};

// Mockup "reverse_dispatch"
namespace detail
{

template <typename Tag1, typename Tag2>
struct reverse_dispatch : false_type {};

// Reverse ring/point:
template <> struct reverse_dispatch<ring_tag, point_tag> : true_type {};

}

template <typename Geometry1, typename Geometry2>
struct reverse_dispatch : detail::reverse_dispatch
    <
        typename tag<Geometry1>::type,
        typename tag<Geometry2>::type
    >
{};


// Mockup strategy tags
struct strategy_tag_distance_point_point {};
struct strategy_tag_distance_point_segment {};


namespace strategy { namespace distance { namespace services
{

template <typename Strategy> struct tag {};
template <typename Strategy> struct returning_type {};

template
<
    typename GeometryTag,
    typename Point1,
    typename Point2 = Point1,
    typename CsTag1 = cartesian_tag,
    typename CsTag2 = cartesian_tag,
    typename UnderlyingStrategy = void
>
struct default_strategy {};

}}}


// Two mockup strategies
template
<
    typename Point1,
    typename Point2 = Point1,
    typename CalculationType = double
>
struct pythagoras
{
    static inline double apply(Point1 const& p1, Point2 const& p2)
    {
        return 1.0;
    }
};

template
<
    typename Point,
    typename PointOfSegment = Point,
    typename CalculationType = double,
    typename Strategy = void
>
struct projected_point
{
    inline double apply(Point const& p, PointOfSegment const& p1, PointOfSegment const& p2) const
    {
        return 1.0;
    }
};


// Register the mockup strategies:
namespace strategy { namespace distance { namespace services
{


template <typename Point1, typename Point2, typename CalculationType>
struct tag<pythagoras<Point1, Point2, CalculationType> >
{
    typedef strategy_tag_distance_point_point type;
};


template <typename Point1, typename Point2, typename CalculationType>
struct returning_type<pythagoras<Point1, Point2, CalculationType> >
{
    typedef double type;
};



template <typename Point1, typename Point2>
struct default_strategy<point_tag, Point1, Point2, cartesian_tag, cartesian_tag, void>
{
    typedef pythagoras<Point1, Point2> type;
};

template <typename Point, typename PointOfSegment, typename CalculationType, typename Strategy>
struct tag<projected_point<Point, PointOfSegment, CalculationType, Strategy> >
{
    typedef strategy_tag_distance_point_segment type;
};


}}} 



// Helper-function
template <typename Geometry1, typename Geometry2>
struct get_default_strategy
    : strategy::distance::services::default_strategy
          <
              point_tag,
              typename point_type<Geometry1>::type,
              typename point_type<Geometry2>::type
          >
{};


// mockup dispatch of mockup distance function
namespace dispatch
{


// To avoid spurious namespaces here:
// NOTE: THIS MIGHT CAUSE A msvc COMPILE ERROR. SEE BELOW
using strategy::distance::services::returning_type;


template
<
    typename Geometry1, typename Geometry2,
    typename Strategy = typename get_default_strategy<Geometry1, Geometry2>::type,
    typename Tag1 = typename tag<Geometry1>::type,
    typename Tag2 = typename tag<Geometry2>::type,
    typename StrategyTag = typename strategy::distance::services::tag<Strategy>::type,
    bool Reverse = reverse_dispatch<Geometry1, Geometry2>::type::value
>
struct distance
{};


template
<
    typename Geometry1, typename Geometry2, typename Strategy,
    typename Tag1, typename Tag2, typename StrategyTag
>
struct distance
<
    Geometry1, Geometry2, Strategy,
    Tag1, Tag2, StrategyTag,
    true
>
    : distance<Geometry2, Geometry1, Strategy, Tag2, Tag1, StrategyTag, false>
{
    static inline typename returning_type<Strategy>::type apply(
        Geometry1 const& g1,
        Geometry2 const& g2,
        Strategy const& strategy)
    {
        return 0;
    }
};

// If reversal is needed and we got the strategy by default, invert it before
// proceeding to the reversal.
template
<
    typename Geometry1, typename Geometry2,
    typename Tag1, typename Tag2, typename StrategyTag
>
struct distance
<
    Geometry1, Geometry2,
    typename get_default_strategy<Geometry1, Geometry2>::type,
    Tag1, Tag2, StrategyTag,
    true
>
    : distance
          <
              Geometry2, Geometry1,
              typename get_default_strategy<Geometry2, Geometry1>::type,
              Tag2, Tag1, StrategyTag,
              false
          >
{
    typedef typename get_default_strategy<Geometry2, Geometry1>::type strategy_type;

    // THIS IS IT: if changed into e.g. _MSC_VER_BUG it stops compiling for MSVC
    // It is is _MSC_VER it compiles fine.
#if defined(_MSC_VER)
    typedef typename strategy::distance::services::returning_type<strategy_type>::type the_return_type; 
#else
    typedef typename returning_type<strategy_type>::type the_return_type; 
#endif

    static inline the_return_type apply(Geometry1 const& g1, Geometry2 const& g2, strategy_type const& strategy)
    {
#if defined(_MSC_VER)
        // This should NOT compile but it does.
        std::cout << typeid(returning_type).name() << std::endl;
#endif
        return 1;
    }
};

// Point-ring , where point-segment strategy is specified
template <typename Point, typename Ring, typename Strategy>
struct distance
<
    Point, Ring, Strategy,
    point_tag, ring_tag, strategy_tag_distance_point_point,
    false
>
{
    typedef typename returning_type<Strategy>::type returning_type;

    static inline returning_type apply(Point const& point,
            Ring const& ring,
            Strategy const& strategy)
    {
        return 0;
    }
};

}

// Mockup distance functions

template <typename Geometry1, typename Geometry2, typename Strategy>
inline typename strategy::distance::services::returning_type
        <
            Strategy
        >::type distance_with_strategy(
                Geometry1 const& geometry1, Geometry2 const& geometry2, Strategy const& strategy)
{
    return dispatch::distance
               <
                   Geometry1,
                   Geometry2,
                   Strategy
               >::apply(geometry1, geometry2, strategy);
}


template <typename Geometry1, typename Geometry2>
inline typename strategy::distance::services::returning_type
        <
            typename get_default_strategy<Geometry1, Geometry2>::type
        >::type distance(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    return distance_with_strategy(geometry1, geometry2,
                    typename get_default_strategy<Geometry1, Geometry2>::type());
}



// Mockup models
namespace model
{

template<typename CoordinateType>
struct point
{
    point(double x, double y) {}
};

template<typename P>
struct ring
{
    void push_back(P const& p) {}
};

}

// Register mocked up models
namespace traits
{
    template<typename T> struct tag<model::point<T> > { typedef point_tag type; };
    template<typename P> struct tag<model::ring<P> > { typedef ring_tag type; };
    
    template<typename T> struct point_type<model::point<T> > { typedef model::point<T> type; };
    template<typename P> struct point_type<model::ring<P> > { typedef P type; };
}

}}


int main()
{
    typedef boost::geometry::model::point<double> P;

    boost::geometry::model::ring<P> sample;
    sample.push_back(P(0, 0));
    sample.push_back(P(0, 3));
    sample.push_back(P(3, 0));
    sample.push_back(P(0, 0));
    P p(10, 10);

    std::cout << boost::geometry::distance(sample, p) << std::endl;
    return 0;
}

