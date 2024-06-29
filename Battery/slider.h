#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
class Slider : public QWidget
{
    Q_OBJECT
public:
    explicit Slider(double minValue,double maxValue,QWidget *parent = nullptr);
    double position;
protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void paintEvent(QPaintEvent * event)override;
private:
    double sliderMinValue;
    double sliderMaxValue;
        QPoint startPos;
    bool isDraging;

    double  currentValue;
signals:
    void positionChanged(double position);
};

#endif // SLIDER_H
