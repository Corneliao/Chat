#include "../include/window.h"
#include "../include/clientwork.h"

Window::Window(QWidget *parent) : QWidget(parent) {
    this->iniUI();
}

Window::~Window() {
}

void Window::iniUI() {
    this->setFixedSize(1000, 600);
    QQuickWidget *qml_widget = new QQuickWidget(this);
    qml_widget->setSource(QUrl("qrc:/Resource/main.qml"));
    // qml rootobject
    this->root_object = qml_widget->rootObject();
    // qml left toolbar  object
    this->tool_bar_object = this->root_object->findChild<QObject *>("toolbar");
    // conaiiner object
    this->container_object = this->root_object->findChild<QObject *>("container");
    // userlogin object
    this->userLogin_object = this->root_object->findChild<QObject *>("userlogin");
    // connectServerState
    connect(this->userLogin_object, SIGNAL(getUserName(QString)), SLOT(getClientUserName(QString)), Qt::DirectConnection);
    //  set width and height
    this->root_object->setProperty("width", this->width());
    this->root_object->setProperty("height", this->height());
}

void Window::create_thread() {
    this->thread = new QThread;
    this->work = new clientWork;
    work->moveToThread(thread);
    thread->start();

    // connections
    // inform thread connection server
    connect(this, &Window::informthreadConnectionServer, work, &clientWork::thread_connectionServer, Qt::QueuedConnection);
    // connectionserver succeed;
    connect(work, &clientWork::thread_connectServerState, this, &::Window::connectServerState, Qt::QueuedConnection);
    // user active disconnectserver
    connect(this->container_object, SIGNAL(qmldisconnectionServer()), work, SLOT(thread_ActivedisconnectedServer()), Qt::QueuedConnection);
    // user disconnectServer and delete the thread
    connect(work, &clientWork::thread_disconnectedServer, this, &Window::delete_thread, Qt::QueuedConnection);
    // receive client textarea message
    connect(this->container_object, SIGNAL(qmlsendMessage(QString)), work, SLOT(thread_sendQmlMessage(QString)), Qt::QueuedConnection);
    // update message to  Qml
    connect(work, &clientWork::thread_updateClientMessageToQml, this, &Window::updateMessageToQml, Qt::QueuedConnection);
    // user send file and get the file path
    connect(this->container_object, SIGNAL(qmlSendFilePath(QString)), work, SLOT(thread_getUserFilePath(QString)), Qt::QueuedConnection);
    // update clientnumbers
    connect(work, &clientWork::thread_updateClientNumbers, this, &Window::updateClientNumbers, Qt::QueuedConnection);
    // update file transfer progress
    connect(work, &clientWork::thread_updateProgress, this, &Window::updateProgress, Qt::QueuedConnection);
    // update loginInfo
    connect(work, &clientWork::thread_updateClientLoinInfoToQml, this, &Window::updateClientLoginInfoToQml, Qt::QueuedConnection);
}

void Window::delete_thread() {
    this->thread->quit();
    this->thread->wait();
    this->thread->deleteLater();
    this->work->deleteLater();
    this->thread = nullptr;
    this->work = nullptr;
}
void Window::getClientUserName(const QString &username) {
    this->userName = username;
    this->create_thread();
    emit this->informthreadConnectionServer(username);
}

void Window::connectServerState(const bool flag) {
    const QVariant data(flag);
    QMetaObject::invokeMethod(this->userLogin_object, "connectionServerState", Q_ARG(QVariant, data));
    if (flag) {
        this->tool_bar_object->setProperty("headtitleName", this->userName);
        this->container_object->setProperty("userName", this->userName);
    } else {
        this->delete_thread();
    }
}

void Window::updateMessageToQml(const QString &message) {
    const QVariant data(message);
    QMetaObject::invokeMethod(this->container_object, "updateChatWindow", Q_ARG(QVariant, data));
}

void Window::updateClientNumbers(const QString &clientSize) {
    const QVariant size(clientSize);
    QMetaObject::invokeMethod(this->container_object, "updateClientNumbers", Q_ARG(QVariant, size));
}

void Window::updateProgress(const QString &progress) {
    const QVariant data(progress);
    QMetaObject::invokeMethod(this->container_object, "setProgress", Q_ARG(QVariant, data));
}

void Window::updateClientLoginInfoToQml(const QString &message) {
    const QVariant loginInfo(message);
    QMetaObject::invokeMethod(this->container_object, "updateClientLoginInfo", Q_ARG(QVariant, loginInfo));
}
