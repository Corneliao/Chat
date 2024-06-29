#include "myslider.h"

MySlider::MySlider(int sliderWidth, int sliderHeight, QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(sliderWidth,sliderHeight);
    this->width = sliderWidth;
    this->height = sliderHeight;

}

void MySlider::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing  | QPainter::TextAntialiasing);

    paint.save();
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(144,238,144)));
    paint.drawRoundedRect(0,0,this->width,this->height,5,5);
    paint.restore();
}
