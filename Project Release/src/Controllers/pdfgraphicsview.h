#ifndef PDFGRAPHICSVIEW
#define PDFGRAPHICSVIEW

#include <QGraphicsView>
#include <QString>
#include <QMouseEvent>
#include <QPoint>
#include <QScrollBar>
#include <cmath>

#include "essaypsgradingscreen.h"
#include "../Helpers/debug.h"


namespace Ui {
class PDFGraphicsView;
}

class PDFGraphicsView : public QGraphicsView {
public:
    explicit PDFGraphicsView(QWidget *parent = 0);
    //PDFGraphicsView(QWidget *parent = 0, EssayPSGradingScreen * _container);
    void mousePressEvent(QMouseEvent * event);
    void setContainer(EssayPSGradingScreen * _container);
    void setScrollPosition(int _x, int _y);
    int getVerticalScrollBarPosition();
    int getHorizontalScrollBarPosition();
    QPoint getMousePressedData();
private:
    int mousex;
    int mousey;
    bool pressed;
    EssayPSGradingScreen * container;
};

void PDFGraphicsView::setContainer(EssayPSGradingScreen * _container) {
    container = _container;
}

PDFGraphicsView::PDFGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    mousex = 0;
    mousey = 0;
    pressed = false;
    container = 0;
}

void PDFGraphicsView::mousePressEvent(QMouseEvent *event) {
    //if (pressed == false) {
    if (event->button() == Qt::LeftButton && container != 0) {
        mousex = event->x();
        mousey = event->y();
        pressed = true;
        int calculatedX = 0;
        int calculatedY = 0;
        QGraphicsScene * tempScene = this->scene();
        QRectF rect = tempScene->sceneRect();
        //DEBUG(DEBUG_WARN, rect.width());
        //DEBUG(DEBUG_WARN, rect.height());
        QScrollBar * hor = this->horizontalScrollBar();
        QScrollBar * vert = this->verticalScrollBar();
        float temp = ((float)(hor->sliderPosition() + mousex)) / rect.width() * 100;
        calculatedX = (int)floor(temp);
        temp = ((float)(vert->sliderPosition() + mousey)) / rect.height() * 100;
        calculatedY = (int)floor(temp);
        container->setCommentPosition(calculatedX, calculatedY);
        //container->setCommentPosition(mousex, mousey);
        //DEBUG(DEBUG_WARN, mousex);
        //DEBUG(DEBUG_WARN, mousey);
    }
    QGraphicsView::mousePressEvent(event);
}

QPoint PDFGraphicsView::getMousePressedData() {
    QPoint point;
    point.setX(mousex);
    point.setY(mousey);
    pressed = false;
    return point;
}

int PDFGraphicsView::getHorizontalScrollBarPosition() {
    QScrollBar * hor = this->horizontalScrollBar();
    return hor->sliderPosition();
}

int PDFGraphicsView::getVerticalScrollBarPosition() {
    QScrollBar * vert = this->verticalScrollBar();
    return vert->sliderPosition();
}

void PDFGraphicsView::setScrollPosition(int _x, int _y) {
    QScrollBar * hor = this->horizontalScrollBar();
    QScrollBar * vert = this->verticalScrollBar();
    hor->setSliderPosition(_x);
    vert->setSliderPosition(_y);
}

#endif // PDFGRAPHICSVIEW

