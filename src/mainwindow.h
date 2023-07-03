#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "graphicsscene.h"
#include <QMainWindow>
#include <iomanip>
#include <sstream>
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <iterator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class manages user io.
 *
 * It's a interface between GUI elements.
 * It can also generate a dataset.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selVertButton_clicked();

    void on_actionreset_triggered();
	
    void on_actiongenerate_new_histogramm_triggered();

    void on_nd_pushButton_clicked();

    void on_fd_pushButton_clicked();

    void on_toolButton_zoom_minus_clicked();

    void on_toolButton_zoom_plus_clicked();

    void on_actionreset_rout_triggered();

    void on_rout_pushButton_clicked();

    /* creates a dataset
	 *
     * Generates 3 csv files with randomized data, in the
     * current directory.
     * Can throw a Bad Logic Exception.
     */
    void on_actiongen_Data_triggered();

    void on_importButton_clicked();

    void on_actionoptimal_route_triggered();

    void on_pushButton_clicked();

    void on_importRoutePushButton_clicked();

private:
    Ui::MainWindow *ui;
    GraphicsScene *mScenePtr;
};
#endif // MAINWINDOW_H
