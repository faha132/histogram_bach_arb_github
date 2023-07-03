#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "hist.h"
#include "rout_scheme.h"
#include "short_path.h"
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QList>
#include <QPoint>
#include <QString>
#include <string>

//! The GraphicsScene class controls the visualisation.
/*!
	GraphicsScene formats histogramm data into visualizable structures.
    It also interacts with mainwindow to process user input.
*/
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
//! a constructor.
/*!
	\param parent for inherited constructor
*/
    explicit GraphicsScene(QObject* parent = nullptr);
	
//! preprocesses and stores in member variables.
/*!
  \param hist contains the new data
*/
    void set_hist(Hist hist);

//! adds colored lines, circles, and describtions 
/*!
  \param v_id source vertex for additions
*/	
    void highlight_v(int v_id);

//! removes graphic items
    void remove_hist();

//! removes graphic items
    void reset_highlight();

//! adds colored lines, circles, and describtions to near dominator
/*!
    \param s source vertex
    \param t target vertex
*/
    void highlight_nd(int s, int t);

//! adds colored lines, circles, and describtions to far dominator
/*!
    \param s source vertex
    \param t target vertex
*/
    void highlight_fd(int s, int t);


/*! \fn void draw_rout(std::list<t_i_s> rout_v_ptrs)
    \brief adds colored lines, circles, and describtions from a route
    \param rout_v_ptrs IDs and decision descriptions, for routing steps
*/
    using t_i_s = std::tuple<int, std::string>;
    void draw_rout(std::list<t_i_s> rout_v_ptrs);

    /*! \fn void draw_only_rout(std::list<t_i_s> rout_v_ptrs)
     * \brief same as draw_rout, but removes the histogramm
     * \param rout_v_ptrs IDs and decision descriptions, for routing steps
     */
    void draw_only_rout(std::list<t_i_s> rout_v_ptrs);

//! removes colored lines, circles, and describtions from current route        
    void reset_rout();

    /*!
     * \brief getter function
     * \return
     */
    Rout_scheme rScheme() const;

    /*!
     * \brief shortest path by dijkstra
     * \return
     *
     * draws in another color for side by side comparison
     */
    void draw_opt_path(int s, int t);



private:
    int mImage_scale = 10;
    QList<QGraphicsItem*> mGra_Itms_ptrs;
    QGraphicsPolygonItem * mHist_Itm;
    Hist mHist;
    std::vector<QGraphicsSimpleTextItem *> mTexts_ptrs;
    std::vector<int> mTextNoChanged;
    QList<QGraphicsItem*> mRout_Gra_Itms_ptrs;
    Rout_scheme mRScheme;
    std::map<int,int> mBr_map;
    std::unique_ptr<Short_path> mSh_path = nullptr;
};

#endif // GRAPHICSSCENE_H
