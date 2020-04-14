#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <iostream>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::box<point_t> box_t;
typedef std::vector<point_t> cluster_t;

// used in the rtree constructor with Boost.Range adaptors
// to generate std::pair<point_t, std::size_t> from point_t on the fly
template <typename First, typename Second>
struct pair_generator
{
    typedef std::pair<First, Second> result_type;
    template<typename T>
    inline result_type operator()(T const& v) const
    {
        return result_type(v.value(), v.index());
    }
};

// used to hold point-related information during clustering
struct point_data
{
    point_data() : used(false) {}
    bool used;
};

// find clusters of points using cluster radius r
void find_clusters(std::vector<point_t> const& points,
                   double r,
                   std::vector<cluster_t> & clusters)
{
    typedef std::pair<point_t, std::size_t> value_t;
    typedef pair_generator<point_t, std::size_t> value_generator;

    if (r < 0.0)
        return; // or return error

    // create rtree holding std::pair<point_t, std::size_t>
    // from container of points of type point_t
    bgi::rtree<value_t, bgi::rstar<4> >
        rtree(points | boost::adaptors::indexed()
                     | boost::adaptors::transformed(value_generator()));

    // create container holding point states
    std::vector<point_data> points_data(rtree.size());

    // for all pairs contained in the rtree
    for(auto const& v : rtree)
    {
        // ignore points that were used before
        if (points_data[v.second].used)
            continue;

        // current point
        point_t const& p = v.first;
        double x = bg::get<0>(p);
        double y = bg::get<1>(p);

        // find all points in circle of radius r around current point
        std::vector<value_t> res;
        rtree.query(
            // return points that are in a box enclosing the circle
            bgi::intersects(box_t{{x-r, y-r},{x+r, y+r}})
            // and were not used before
            // and are indeed in the circle
            && bgi::satisfies([&](value_t const& v){
                   return points_data[v.second].used == false
                       && bg::distance(p, v.first) <= r;
            }),
            std::back_inserter(res));

        // create new cluster
        clusters.push_back(cluster_t());
        // add points to this cluster and mark them as used
        for(auto const& v : res) {
            clusters.back().push_back(v.first);
            points_data[v.second].used = true;
        }
    }
}

int main()
{
    std::vector<point_t> points;

    for (double x = 0.0 ; x < 10.0 ; x += 1.0)
        for (double y = 0.0 ; y < 10.0 ; y += 1.0)
            points.push_back(point_t{x, y});

    std::vector<cluster_t> clusters;

    find_clusters(points, 3.0, clusters);

    for(size_t i = 0 ; i < clusters.size() ; ++i) {
        std::cout << "Cluster " << i << std::endl;
        for (auto const& p : clusters[i]) {
            std::cout << bg::wkt(p) << std::endl;
        }
    }
}
