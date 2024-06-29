#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "batteryrect.h"
#include "batterycolorrect.h"
#include "myslider.h"
#include "slider.h"
#include <QCheckBox>
#include <QPropertyAnimation>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent * event) override;
private:
    Ui::Widget *ui;

    double centerX;
    double centerY;
    QPoint leftTop;
    QWidget * batteryWidget;
    int batteryWidget_width;
    int batteryWidget_height;
 //   QWidget * batteryRect;
    BatteryRect *  batteryRect;
    BatteryColorRect * batteryColorRect;

    QWidget * sliderWidget;
    MySlider * sliderRect;
    Slider * slider;

    QString batteryText;

    QCheckBox * checkBox;

    QPropertyAnimation   *animation;
};
#endif // WIDGET_H
