#include "widget.h"
#include "./ui_widget.h"
#include <QLabel>
#include <QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->batteryWidget_width = 200;
    this->batteryWidget_height = 100;
    this->centerX = this->rect().center().x();
    this->centerY = this->rect().center().y();
    this->leftTop = QPoint(this->rect().center().x()-this->batteryWidget_width/2,this->rect().center().y()-this->batteryWidget_height/2);
    //电池区域
    this->batteryWidget = new QWidget(this);
    batteryWidget->setFixedSize(this->batteryWidget_width,this->batteryWidget_height);
    batteryWidget->move(QPoint(this->rect().center().x()-this->batteryWidget_width/2,this->rect().center().y()-this->batteryWidget_height/2));
    batteryWidget->setStyleSheet("background-color:gray;border-radius:8px;");
    //电池体
    this->batteryRect = new BatteryRect(this->batteryWidget_width,this->batteryWidget_height,this->batteryWidget);
    //电池块
    this->batteryColorRect = new BatteryColorRect(7,this->batteryWidget_width,this->batteryWidget_height,this->batteryRect);
    //滑动条区域
    this->sliderWidget = new QWidget(this);
    this->sliderWidget->setFixedSize(200,10);
    sliderWidget->move(QPoint(this->batteryWidget->frameGeometry().bottomLeft().x(),this->batteryWidget->frameGeometry().bottomLeft().y()+30));
    sliderWidget->setStyleSheet("background-color:gray;border-radius:5;");

    this->sliderRect = new MySlider(this->batteryWidget_width,10,this->sliderWidget);
    this->slider = new Slider(this->sliderWidget->frameGeometry().topLeft().x(),this->sliderWidget->frameGeometry().topRight().x()-17,this);
    this->slider->move(QPoint(this->sliderWidget->frameGeometry().x(),this->sliderWidget->frameGeometry().y()-4));
    QLabel * label = new QLabel(this);
    label->setFixedSize(100,30);
    label->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setBold(true);
    label->setFont(font);
    label->move(this->rect().center().x()-label->width()/2,this->rect().center().y()-label->height()/2);
    int kk = this->batteryColorRect->position()*100;
    label->setText(QString::number(kk)+"%");

    //动画复选框
    this->checkBox = new QCheckBox(this);
    this->checkBox->setText("开启动画");
    this->checkBox->move(QPoint(this->sliderWidget->frameGeometry().bottomRight().x()+20,this->sliderWidget->frameGeometry().bottomRight().y()-this->checkBox->height()/2));

    //动画类

    this->animation = new QPropertyAnimation(this->batteryColorRect,"position",this);
    this->animation->setDuration(1000);
    QPushButton * btn  =new QPushButton("开始动画 ",this);
    connect(btn,&QPushButton::clicked,this,[=](){
        this->animation->start();
    });
    connect(this->batteryColorRect,&BatteryColorRect::positionChanged,this,[=](double  position){
        if (kk <= 20) {
            this->batteryColorRect->setBatteryColor(QColor(Qt::red));
        } else if (kk > 20 && kk < 50) {
            this->batteryColorRect->setBatteryColor(QColor(Qt::yellow));
        } else {
            this->batteryColorRect->setBatteryColor(QColor(144, 238, 144));
        }
        int kk = this->batteryColorRect->position() * 100;
        label->setText(QString::number(kk) + "%");
        //this->batteryColorRect->update();
        qDebug() << this->batteryColorRect->position()  << this->slider->position;
    });
    connect(this->slider, &Slider::positionChanged, this, [=](double pos) {
        if(this->checkBox->checkState() == Qt::Unchecked)  {
            this->batteryColorRect->setPosition(pos);
            qDebug() << "main" << pos;
            this->batteryColorRect->update();
            // 变换 颜色
            if (kk <= 20) {
                this->batteryColorRect->setBatteryColor(QColor(Qt::red));
            } else if (kk > 20 && kk < 50) {
                this->batteryColorRect->setBatteryColor(QColor(Qt::yellow));
            } else {
                this->batteryColorRect->setBatteryColor(QColor(144, 238, 144));
            }
            int kk = this->batteryColorRect->position() * 100;
            label->setText(QString::number(kk) + "%");
        }
        else {
            this->animation->setStartValue(this->batteryColorRect->position());
            this->animation->setEndValue(this->slider->position);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

