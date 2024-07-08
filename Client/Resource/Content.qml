import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs
import QtQuick.Layouts

Rectangle {
    property string userName: "Flache"
    property string onlineMembers: "0"
    property string disconnectButtonName: "连接服务器"
    signal qmlsendMessage(string str)
    signal qmlSendFilePath(string path)
    signal qmldisconnectionServer
    signal connectionState
    color: "#f0f0f0"
    visible: true
    id: container
    //顶栏
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.topMargin: 10
        width: parent.width
        height: 40
        radius: 10
        color: "#f0f0f0"
        //用户名标签
        Rectangle {
            id: userName
            width: 100
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            color: "white"
            radius: 25
            anchors.left: parent.left
            anchors.leftMargin: 10
            Text {
                text: container.userName
                font.bold: true
                anchors.centerIn: parent
                color: "#8E9EAB"
            }
        }
        //在线人数标签
        Rectangle {
            id: onlineMembers
            width: 150
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            color: "white"
            radius: 25
            anchors.left: userName.right
            anchors.leftMargin: 10
            Text {
                text: "当前在线人数：" + container.onlineMembers
                font.bold: true
                anchors.centerIn: parent
                color: "#8E9EAB"
            }
        }
        Rectangle {
            id: disconnectButton
            width: 150
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            height: parent.height
            radius: 25
            Text {
                text: disconnectButtonName
                font.bold: true
                color: "#8E9EAB"
                anchors.centerIn: parent
            }
            NumberAnimation on scale {
                id: disconnectionbuttonAnimation1
                to: 0.5
                duration: 20
                running: false
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    disconnectionbuttonAnimation1.to = 0.8
                    disconnectionbuttonAnimation1.start()
                    container.qmldisconnectionServer()
                    container.disConnection()
                    container.connectionState()
                }
                onReleased: {
                    disconnectionbuttonAnimation1.to = 1
                    disconnectionbuttonAnimation1.start()
                }
            }
        }
    }

    Rectangle {
        id: loginInfo
        radius: 20
        width: parent.width / 2
        height: 160
        anchors.right: messageContainer.left
        anchors.rightMargin: 20
        anchors.top: topBar.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        visible: false
        opacity: 0
        TextArea {
            id: loginInfotext
            anchors.fill: parent
            placeholderText: "登录信息"
            color: "#8E9EAB"
            anchors.margins: 10
        }
    }
    Rectangle {
        id: fileinfo
        radius: 20
        anchors.top: loginInfo.bottom
        anchors.topMargin: 20
        width: parent.width / 2
        visible: false
        opacity: 0
        anchors.right: messageContainer.left
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        height: 160
        Text {
            id: fileInfotext
            anchors.centerIn: parent
            color: "#8E9EAB"
            text: "文件传输"
            anchors.margins: 10
        }
    }

    //animation

    //显示消息区域
    Rectangle {
        id: messageContainer
        anchors.top: topBar.bottom
        anchors.topMargin: 10
        radius: 20
        width: parent.width - 20
        height: 340
        anchors.right: parent.right
        anchors.leftMargin: 10
        ScrollView {
            id: scrollView
            clip: true
            anchors.fill: parent
            anchors.margins: 10
            TextArea {
                id: textarea
                readOnly: true
                textFormat: TextEdit.RichText
                font.pixelSize: 13
                placeholderText: "消息"
                placeholderTextColor: "#8E9EAB"
                color: "#8E9EAB"
            }
        }
    }
    //发送文件
    Rectangle {
        width: 100
        height: 40
        color: "white"
        radius: 25
        anchors.top: messageContainer.bottom
        anchors.right: messageContainer.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
        Text {
            text: "发送文件"
            anchors.centerIn: parent
            color: "#8E9EAB"
            font.bold: true
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                filetrans_animation.to = 0.8
                filetrans_animation.start()
                fileInfotext.text = "等待接受...."
            }
            onReleased: {
                filetrans_animation.to = 1
                filetrans_animation.start()
                fileDialog.open()
            }
        }
        NumberAnimation on scale {
            id: filetrans_animation
            to: 0.8
            duration: 20
            running: false
        }

        FileDialog {
            id: fileDialog
            title: "选择文件"
            nameFilters: "All(*)"
            fileMode: FileDialog.OpenFile
            onAccepted: {
                container.qmlSendFilePath(fileDialog.currentFile)
            }
        }
    }
    //发送消息文本区域
    Rectangle {
        id: sendmessageContainer
        radius: 20
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.leftMargin: 10
        width: parent.width - 20
        height: 120
        TextArea {
            id: sendtextArea
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            width: parent.width
            height: parent.height - 20
            clip: true
        }
        //发送消息按钮
        Rectangle {
            color: "#f0f0f0"
            width: 100
            height: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            radius: 25
            NumberAnimation on scale {
                id: animation
                to: 0.8
                duration: 20
                running: false
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    cursorShape: Qt.PointingHandCursor
                }
                onPressed: {
                    animation.to = 0.8
                    animation.start()
                    container.qmlsendMessage(sendtextArea.text)
                    sendtextArea.clear()
                }
                onReleased: {
                    animation.to = 1
                    animation.start()
                }
            }
            Text {
                text: "发送"
                color: "#8E9EAB"
                font.bold: true
                anchors.centerIn: parent
            }
        }
    }

    SequentialAnimation {
        running: false
        id: connectionForwardAnimation
        NumberAnimation {
            target: messageContainer
            property: "width"
            to: parent.width / 2
            duration: 500
        }
        PropertyAnimation {
            target: loginInfo
            property: "visible"
            to: true
            duration: 1
        }
        PropertyAnimation {
            target: loginInfo
            property: "opacity"
            to: 1
            duration: 500
        }
        PropertyAnimation {
            target: fileinfo
            property: "visible"
            to: true
            duration: 1
        }
        PropertyAnimation {
            target: fileinfo
            property: "opacity"
            to: 1
            duration: 500
        }
    }
    SequentialAnimation {
        id: connectionBacklAnimation
        running: false
        PropertyAnimation {
            target: loginInfo
            property: "opacity"
            to: 0
            duration: 500
        }
        PropertyAnimation {
            target: loginInfo
            property: "visible"
            to: false
            duration: 1
        }

        PropertyAnimation {
            target: fileinfo
            property: "opacity"
            to: 0
            duration: 500
        }
        PropertyAnimation {
            target: fileinfo
            property: "visible"
            to: false
            duration: 1
        }
        NumberAnimation {
            target: messageContainer
            property: "width"
            to: parent.width - 20
            duration: 500
        }
        onStopped: {
            loginInfotext.clear()
            textarea.clear()
            fileInfotext.text = "文件传输"
        }
    }
    //更新客户端消息
    function updateChatWindow(str) {
        textarea.append(str)
    }

    function updateClientLoginInfo(login) {
        connectionForwardAnimation.start()
        loginInfotext.append(login)
        container.disconnectButtonName = "断开服务器"
    }

    //客户端断开连接
    function disConnection() {
        loginInfotext.append("你已断开服务器")
        connectionBacklAnimation.start()
        container.disconnectButtonName = "连接服务器"
    }
    //更新客户端数量
    function updateClientNumbers(nums) {
        container.onlineMembers = nums
    }
    //更新进度
    function setProgress(progress) {
        fileInfotext.text = progress + "%"
    }
}
