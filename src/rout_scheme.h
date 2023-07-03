#ifndef ROUT_SCHEME_H
#define ROUT_SCHEME_H

#include "rout_tab.h"
#include "hist.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>

/*!
 * \brief The Rout_scheme class is the collection of the routing tables.
 *
 * It can collect data about tables.
 * It can compute a route.
 */

class Rout_scheme
{
public:
    /*!
     * \brief Rout_scheme constructor
     * \param h grabs the visibile points and amount of vertices for each
     */
    Rout_scheme(Hist h);

    /*!
     * \brief rout simulates the routing algorithm via adressing nodes
     * \param s source vertex ID
     * \param t target vertex ID
     * \return tuples from visited vertices, with vertex id and describtion
	 *
     * Algorithm taken from Max PHD thesis.
     * The description is, about why, this was the next vertex.
     */
    const std::list<std::tuple<int, std::string>> rout(int s, int t) const;

    /*!
     * \brief tables getter
     * \return routing tables representing nodes
	 *
     * is used to grab data to csv
     */
    const std::vector<Rout_tab>& tables() const;

    /*!
     * \brief find_brs calculates all vertex breakpoints
     * \return allows binary search
	 *
     * Algorithm taken from Max PHD Thesis.
     * Used for creating the routing tables and for the visualization.
     * Cannot use tables for breakpoint visualisation, they contain only
     * breakpoints of visible vertices.
     */
    const std::map<int,int> find_brs() const;

    /*!
     * \brief print_tables_to_file currently called example.txt
	 *
     * is about the current visualized histogramm
     */
    void print_tables_to_file() const;

    /*!
     * \brief tables_to_csv_data helper function
     * \return into data_hists_verts.csv
     */
    std::vector<std::string> tables_to_csv_data() const;

    /*!
     * \brief rout_data_string similar to rout function
     * \param s source vertex ID
     * \param t target vertex ID
     * \return route with additional data for data_rout_steps_t.csv
     */
    const std::vector<std::pair<std::vector<int>, double>> 
		rout_data_string(int s, int t) const;

private:
    std::vector<Rout_tab> mTables;
    int mEntriesCount;
    int mEncodSize;
    int mEntriesMax;
};

#endif // ROUT_SCHEME_H
