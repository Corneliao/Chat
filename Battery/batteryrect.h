#ifndef BATTERYRECT_H
#define BATTERYRECT_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
class BatteryRect : public QWidget
{
    Q_OBJECT
public:
    explicit BatteryRect(int batteryWidth,int batteryHeight,QWidget *parent = nullptr);
protected:
   void paintEvent(QPaintEvent * event)override;
private:
    int width;
    int height;
    QPoint topLeft;
    QPoint bottomRight;
signals:
};

#endif // BATTERYRECT_H
