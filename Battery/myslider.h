#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QPainter>
class MySlider : public QWidget
{
    Q_OBJECT
public:
    explicit MySlider(int sliderWidth,int sliderHeight,QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent * event) override;
private:
    int width;
    int height;
signals:
};

#endif // MYSLIDER_H
