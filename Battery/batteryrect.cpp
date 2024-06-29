#include "batteryrect.h"

BatteryRect::BatteryRect(int batteryWidth, int batteryHeight, QWidget *parent)
    : QWidget{parent}
{

    this->topLeft =QPoint(this->rect().x()+7,this->rect().y()+7);
    this->bottomRight =  QPoint(this->rect().bottomRight().x()-7,this->rect().bottomRight().y()-7);


    this->width = batteryWidth-14;
    this->height = batteryHeight-14;
    this->setGeometry(topLeft.x(),topLeft.y(),this->width,this->height);
}

void BatteryRect::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    //抗锯齿
    paint.setRenderHints(QPainter::Antialiasing  | QPainter::TextAntialiasing);

    paint.save();
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::NoBrush);
    paint.drawRoundedRect(this->rect(),8,8);
    paint.restore();
}






