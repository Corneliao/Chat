#include "batterycolorrect.h"

BatteryColorRect::BatteryColorRect(int marginValue, int marginBatteryWidth, int marginBatteryHeight, QWidget *parent)
    : QWidget{parent}
{
    this->margin   = marginValue;
    this->width =  marginBatteryWidth-14;
    this->height = marginBatteryHeight-14;
    this->setFixedSize(this->width,this->height);
    this->_position = 0.0;
    this->batteryColor = QColor(144, 238, 144);
}

void BatteryColorRect::setBatteryColor(QColor color)
{
    this->batteryColor = color;
    this->update();
}

void BatteryColorRect::setPosition(double positionValue)
{
    if(this->_position != positionValue) {
        emit this->positionChanged(position());
    }
    this->_position = positionValue;
    this->update();
    qDebug() << "接收值：" << this->_position;
}

double BatteryColorRect::position() const
{
    qDebug() << "返回值" << this->_position;
    if(this->_position != this->_position)  {
        qDebug() << "oo";
    }
    return this->_position;
}


void BatteryColorRect::paintEvent(QPaintEvent *evnet)
{
    QPainter paint(this);
    paint.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制电池块
    paint.save();
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(this->batteryColor));
    QPoint topLeft = QPoint((this->rect().topLeft().x()),this->rect().topLeft().y());
    QPoint bottomRight = QPoint((this->parentWidget()->rect().bottomRight().x())*this->_position-1,this->parentWidget()->rect().bottomRight().y());
    QRect rect = QRect(topLeft,bottomRight);
    paint.drawRoundedRect(rect,8,8);
    paint.restore();

}
