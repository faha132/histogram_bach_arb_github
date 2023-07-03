#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

//!The GraphicsView class only adds zooming to its base class.
class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

public slots:
    void zoomIn();
    void zoomOut() { scale(1 / 1.2, 1 / 1.2); }

private:
    bool highlight_flag;
    const int image_scale = 10;
};

#endif // GRAPHICSVIEW_H
