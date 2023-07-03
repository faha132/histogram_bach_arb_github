#include "short_path.h"


// https://www.boost.org/doc/libs/1_81_0/libs/graph/doc/quick_tour.html

Short_path::Short_path(const std::vector<Rout_tab> & tables)
{
    typedef std::pair<int,int> E;
    std::set<E> edges;
    for (size_t fromVId = 0; fromVId < tables.size(); ++fromVId) {
        for (auto toVId : tables.at(fromVId).find_neighbors()) {
            edges.insert(E(fromVId,toVId));
        }
    }
    std::vector<int> weights(edges.size(), 1);
    mGraph = Graph(edges.begin(), edges.end(), weights.begin(),
                   tables.size());
}

std::tuple<int,std::list<std::string>>
    Short_path::distance(int s, int t)
{
    std::vector<int> d(num_vertices(mGraph));
    typedef boost::graph_traits<Graph>::vertex_descriptor  Vertex;
//https://www.boost.org/doc/libs/1_81_0/libs/graph/example/dijkstra-example.cpp
    Vertex v_s = boost::vertex(s,mGraph);
    std::vector< Vertex > p(num_vertices(mGraph));
    boost::dijkstra_shortest_paths(
                mGraph, v_s,
                predecessor_map(
                    boost::make_iterator_property_map(
                        p.begin(), get(boost::vertex_index, mGraph)))
                .distance_map(
                    boost::make_iterator_property_map(
                        d.begin(), get(boost::vertex_index, mGraph))));
    std::list<std::string> rout_to_t;
    int acc = t;
    while (acc != s) {
        rout_to_t.push_front(std::to_string(acc));
        acc = p.at(acc);
    }
    rout_to_t.push_front(std::to_string(s));
    return std::tuple(d[t], rout_to_t);
}

