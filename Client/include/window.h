#ifndef WINDOW_H
#define WINDOW_H

#include "clientwork.h"
#include <QFileDialog>
#include <QPushButton>
#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>
#include <QTextBrowser>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
class Window : public QWidget {
    Q_OBJECT

    public:
    Window(QWidget *parent = nullptr);
    ~Window();
    void iniUI();
    void create_thread();
    void delete_thread();
    public slots:
    void getClientUserName(const QString &username);
    void connectServerState(const bool flag);
    void updateMessageToQml(const QString &message);
    void updateClientNumbers(const QString &clientSize);
    void updateProgress(const QString &progress);
    void updateClientLoginInfoToQml(const QString &message);
    signals:
    void sendFilePath(const QString &filepath);
    void informthreadConnectionServer(const QString &username);

    private:
    QPushButton *connection_button;
    QPushButton *select_file_button;
    QPushButton *send_filepath_button;
    QTextBrowser *browser;
    QString userName;
    // thread and work
    QThread *thread;
    clientWork *work;
    /*qml properties*/
    QObject *root_object;
    QObject *tool_bar_object;
    QObject *container_object;
    QObject *userLogin_object;
};
#endif // WINDOW_H
