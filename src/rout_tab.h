#ifndef ROUT_TAB_H
#define ROUT_TAB_H

#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <list>

/*!
 * \brief The Rout_tab class can compute a routing step.
 *
 * Data structure taken from Max PHD Thesis.
 * It is equivalent to a node in the network.
 *
 */
class Rout_tab
{
public:
    /*!
     * \brief Rout_tab empty constructor
     */
    Rout_tab();

    /*!
     * \brief Rout_tab standard constructor
     * \param nbrh r-visible vertices IDs
     * \param self_id
     * \param right_base_id needed because of variable histogramm size
     */
    Rout_tab(std::list<int> nbrh, int self_id, int right_base_id);

    /*!
     * \brief set_brs part of construction
     * \param brs from rout_scheme
	 *
     * not in the constructor, to reduce redundancy
     */
    void set_brs(std::map<int, int> &brs);

    /*!
     * \brief self_id getter
     * \return
     */
    int self_id() const;

    /*!
     * \brief entries getter
     * \return r-visible vertices w/ breakpoint
     * and port
     */
    const std::map<int, std::list<int>> &entries() const;

    //rout helpers
    /*!
     * \brief find_l_v computes
     * \return last visible vertex, left of node
     */
    int find_l_v() const;

    /*!
     * \brief find_r_v computes
     * \return last visible vertex, right of node
     */
    int find_r_v() const;

    /*!
     * \brief find_br lookup in mEntries
     * \param v_id visible vertex
     * \return breakpoint of v_id
     */
    int find_br(int v_id) const;

    /*!
     * \brief has_br lookup in mEntries
     * \param v_id visible vertex
     * \return breakpoint of v_id exists?
     */
    bool has_br(int v_id) const;

    /*!
     * \brief find_nd computes
     * \param t second parameter
     * \return near dominator of mSelf_id around t
     */
    const std::map<int, std::list<int>>::const_iterator find_nd(int t) const;

    /*!
     * \brief find_fd computes
     * \param t second parameter
     * \return far dominator of mSelf_id around t
     */
    const std::map<int, std::list<int>>::const_iterator find_fd(int t) const;

    /*!
     * \brief rout_step computes
     * \param t target vertex
     * \return next vertex ID on route and
     * decision ID
     */
    const std::tuple<int, int> rout_step(int t) const;
    // stats
    /*!
     * \brief print_tab_ints helper for Rout_scheme::print_tables_to_file
     * \return formated table data
     */
    const std::string print_tab_ints() const;

    /*!
     * \brief count_entries helper for Rout_scheme::print_tables_to_file()
     * \return
     */
    int count_entries() const;

    /*!
     * \brief retinttostr translates ID
     * \param ret_id 10 different returns
     * \return text describtion
     */
    static const std::string retinttostr(int ret_id);

    // shortest path helper
    /*!
     * \brief find_neighbors lookup in mEntries
     * \return of vertex with mSelf_id
     */
    const std::list<int> find_neighbors() const;

private:
    std::map<int, std::list<int>> mEntries;
    int mSelf_id;
    int mRight_base_id;
};

#endif // ROUT_TAB_H
