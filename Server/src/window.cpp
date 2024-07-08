#include "../include/window.h"
#include <QFileDialog>
Window::Window(QWidget *parent) : QWidget(parent) {
    this->initUI();
    this->initServer();
}

Window::~Window() {
}

void Window::initUI() {
    this->resize(800, 400);
    this->browser = new QTextBrowser(this);
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(this->browser);

    // connections
}

void Window::initServer() {
    this->server = new tcpServer(this);
    if (!server->listen(QHostAddress::Any, quint16(7777))) {
        this->browser->append("listen failed");
        return;
    }
    qDebug() << "listen  succeed";
    connect(this->server, &tcpServer::newClientConnection, this, &Window::dealNewConnection, Qt::DirectConnection);
}

void Window::dealNewConnection(const qintptr handle) {
    QTimer *timer = new QTimer(this);
    timer->setInterval(200);
    // create thread
    QThread *thread = new QThread;
    clientWork *work = new clientWork(handle);
    work->moveToThread(thread);

    // add the thread and work to list
    this->threads.append(thread);
    this->works.append(work);
    // thread start
    thread->start();
    timer->start();
    connect(timer, &QTimer::timeout, this, [=]() {
        work->updateClientNumbers();
        timer->stop();
    });

    // connections
    connect(work, &clientWork::thread_filepath, this, &Window::setAllPath, Qt::QueuedConnection);
    connect(this, &Window::informSendFileData, work, &clientWork::thread_sendFileInfo, Qt::QueuedConnection);
    connect(work, &clientWork::thread_receiveClientMessage, this, &Window::dealClientMessage, Qt::QueuedConnection);
    connect(this, &Window::sendClientMessage, work, &clientWork::thread_sendClientMessage, Qt::QueuedConnection);
    connect(work, &clientWork::thread_clientdisconnectedServer, this, &Window::clientdisconnectedServer, Qt::QueuedConnection);
    connect(work, &clientWork::thread_newClientLoginMessage, this, &Window::showMessageNewLogin, Qt::QueuedConnection);
    connect(work, &clientWork::thread_updateClientNumbers, this, &Window::updateClientNumbers, Qt::QueuedConnection);
    connect(work, &clientWork::thread_updateProgress, this, [=](const QString &progress) { this->browser->append(progress); });
}

void Window::setAllPath(const QString &filepath) {
    for (int i = 0; i < this->works.size(); i++) {
        this->works[i]->thread_setfilepath(filepath);
    }
    emit this->informSendFileData();
}

void Window::dealClientMessage(const int type, const QByteArray &message) {
    emit this->sendClientMessage(type, message);
}

void Window::clientdisconnectedServer(const QString &username) {
    for (int i = 0; i < this->works.size(); i++) {
        if (username == this->works.at(i)->getClientUserName()) {
            this->browser->append(username + "  " + "已断开连接");
            this->threads[i]->quit();
            this->threads[i]->wait();
            this->works[i]->deleteLater();
            this->threads[i]->deleteLater();
            this->threads.remove(i);
            this->works.remove(i);
        }
    }
}

void Window::showMessageNewLogin(const QString &username) {
    this->browser->append(username + " " + "已连接服务器");
}

void Window::updateClientNumbers(const int type) {
    if (type == MESSAGETYPE::userNumbers) {
        QByteArray clientSize = QString::number(this->works.size()).toUtf8();
        emit this->sendClientMessage(type, clientSize);
    }
}

void Window::dealClientdisconnectionServer(const QString &username) {
    QByteArray message = QString(username + " " + "已断开连接").toUtf8();
    int type = MESSAGETYPE::userLogout;
    emit this->sendClientMessage(type, message);
}
