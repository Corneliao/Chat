#include <QApplication>
#include <QQuickView>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QQuickView * view = new QQuickView;
    view->setSource(QUrl("qrc:/Battery.qml"));
    view->show();
    return QApplication::exec();
}
