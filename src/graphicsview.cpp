#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView{parent}
{

}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::zoomIn()
{
    this->scale(1.2, 1.2);
}
