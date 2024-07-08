#ifndef WINDOW_H
#define WINDOW_H

#include "clientwork.h"
#include "tcpserver.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
class Window : public QWidget {
    Q_OBJECT
    enum MESSAGETYPE { userLogin = 0, userMessage, userNumbers, userLogout, userFileInfo, userFileData };

    public:
    Window(QWidget *parent = nullptr);
    ~Window();
    void initServer();
    void initUI();
    public slots:
    void dealNewConnection(const qintptr handle);
    void setAllPath(const QString &filepath);
    void dealClientMessage(const int type, const QByteArray &message);
    void clientdisconnectedServer(const QString &username);
    void showMessageNewLogin(const QString &username);
    void updateClientNumbers(const int type);
    void dealClientdisconnectionServer(const QString &username);
    signals:
    void sendClientMessage(const int type, const QByteArray &message);
    void sendFilePath(const QString &filepath);
    void informSendFileData();

    private:
    tcpServer *server;
    QList<QThread *> threads;
    QList<clientWork *> works;
    QTextBrowser *browser;
    ;
};
#endif // WINDOW_H
