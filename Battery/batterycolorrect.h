#ifndef BATTERYCOLORRECT_H
#define BATTERYCOLORRECT_H

#include <QWidget>
#include <QPainter>
class BatteryColorRect : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged FINAL)
public:
    explicit BatteryColorRect(int margin,int marginBatteryWidth,int marginBatteryHeight,QWidget *parent = nullptr);
    void setBatteryColor(QColor color);
    void setPosition(double positionValue);
    double position()const;
protected:
    void paintEvent(QPaintEvent * evnet);
private:
    int margin;
    int width;
    int height;
    QColor batteryColor;
    double _position;
signals:
    void positionChanged(double  position);

};

#endif // BATTERYCOLORRECT_H
