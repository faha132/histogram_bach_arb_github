#ifndef HIST_H
#define HIST_H

#include <vector>
#include <QPainter>
#include <QWidget>
#include <sstream>
#include <random>
#include <iterator>


//! The Hist class generates and prepocesses histogramms.
/*!
    It creates randomized data for class Rout_scheme to work with.
*/
class Hist
{
public:
    /*!
     * \brief Hist
     * \param size == amount_Vs / 2 - 2
	 *
     * 2 base Vertices are always there,
     * histogramms are always even number on vertices.
     */
    Hist(int size);

    /*!
     * \brief to import Histogramms from csv-Files
     * \param str1 x- and y- coordinates
     *
     * str1 is of the form "x1-coord, y1-c, x2-c, ..."
     */
    Hist(std::vector<std::string> &vec1);

    ~Hist();

    /*!
     * \brief Getter
     * \return contains x- and y coordinates
     */
    const std::vector<QPoint>& getQP_Vs() const;

    /*!
     * \brief visibility_points
     * \param v_id source vertex
     * \return IDs of r-visible vertices
     */
    const std::list<int> visibility_points(int v_id) const;

    /*!
     * \brief scale_invert_QP_Vs preprocesses for GraphicsScene
     * \param image_scale factor
	 *
     * QT has y-Coordinates growing top to bottom
     */
    void scale_invert_QP_Vs(int image_scale);

    /*!
     * \brief verts_to_strings preprocesses for csv-data
     * \return ret[0] = x0, ret[1] = y0, ret[2] = x1, ...
     *
     */
    const std::vector<std::string> verts_to_strings() const;

private:
    std::vector<QPoint> mQP_Vs;
};

#endif // HIST_H
