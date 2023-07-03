#ifndef SHORT_PATH_H
#define SHORT_PATH_H

#include <vector>
#include "rout_tab.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "rout_tab.h"
#include <algorithm>
#include <iterator>

//https://www.boost.org/doc/libs/1_81_0/libs/graph/doc/quick_tour.html
/*!
 * \brief The Short_path class is another routing algorithm for comparison
 *
 * uses Boost graph library
 */
class Short_path
{
public:
    /*!
     * \brief Short_path creates boost::adjacency_list
     * \param tables uses its data
     */
    Short_path(const std::vector<Rout_tab> & tables);

    /*!
     * \brief distance dijkstra sssp
     * \param s source vertex
     * \param t target vertex
     * \return distance and route
     */
    std::tuple<int,std::list<std::string>>
        distance(int s, int t);
private:
    typedef boost::adjacency_list<boost::vecS, boost::vecS,
        boost::directedS, boost::no_property,
        boost::property<
            boost::edge_weight_t, int>
        > Graph;
    Graph mGraph;


};

#endif // SHORT_PATH_H
