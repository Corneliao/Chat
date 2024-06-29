#include "slider.h"

Slider::Slider(double minValue,double maxValue,QWidget *parent)
    : QWidget{parent}
{
    this->currentValue = 0;
    this->isDraging =  false;
    this->sliderMinValue = minValue;
    this->sliderMaxValue  = maxValue;
    this->position = 0.0;
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    this->isDraging = true;
    this->startPos = event->pos();
}

void Slider::mouseReleaseEvent(QMouseEvent *event)
{
    this->isDraging =  false;
}

void Slider::mouseMoveEvent(QMouseEvent *event)
{
    if(this->isDraging) {
        QPoint movePoint = QPoint(event->pos().x()+pos().x()-this->startPos.x(),this->pos().y());
        if(movePoint.x() <this->sliderMinValue) {
            movePoint.setX(this->sliderMinValue);
        }
        if(movePoint.x() > sliderMaxValue) {
            movePoint.setX(sliderMaxValue);
        }
        double tot = this->sliderMaxValue-this->sliderMinValue;
        this->currentValue = movePoint.x()-this->sliderMinValue;
        this->position = this->currentValue/tot;
        this->move(movePoint);
        emit this->positionChanged(this->position);
    }
}

void Slider::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    paint.save();
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(233, 180, 195)));
    paint.drawEllipse(0,0,18,18);
    paint.restore();
}
