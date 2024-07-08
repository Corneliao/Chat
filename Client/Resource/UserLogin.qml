import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Window
import QtQml

Rectangle {
    color: "#f0f0f0"
    property bool isConnectioned: false
    property string username: ""
    signal showContainer
    signal getUserName(string userName)
    signal userdisconnection
    id: root
    TextField {
        id: userNameInput
        width: 200
        height: 40
        anchors.centerIn: parent
        placeholderText: "用户名"
        onTextChanged: {
            if (userNameInput.length !== 0) {
                connectionButton.enabled = true
            } else {
                connectionButton.enabled = false
            }
        }
    }
    Rectangle {
        id: connectionButton
        color: "white"
        width: 120
        height: 40
        radius: 25
        anchors.top: userNameInput.bottom
        anchors.topMargin: 150
        anchors.horizontalCenter: userNameInput.horizontalCenter
        NumberAnimation on scale {
            //   id: animation
            to: 0.8
            duration: 10
            running: false
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                cursorShape: Qt.PointingHandCursor
            }
            onPressed: {
                if (textConnection.text === "连接服务器") {
                    root.getUserName(userNameInput.text)
                    root.username = userNameInput.text
                }
            }
        }
        Text {
            id: textConnection
            text: "连接服务器"
            color: "#8E9EAB"
            font.bold: true
            anchors.centerIn: parent
        }
    }
    Component.onCompleted: {
        if (userNameInput.text.length === 0) {
            connectionButton.enabled = false
        }
    }

    function connectionServerState(flag) {
        if (flag) {
            root.isConnectioned = true
            root.showContainer()
            userNameInput.focus = false
            console.log(flag)
        } else {
            animation.to = root.width - infoRect.width
            animation.start()
            timer.start()
        }
    }

    Rectangle {
        id: infoRect
        width: 200
        height: 30
        radius: 20
        color: "white"
        x: parent.width + 25
        y: parent.height / 2
        NumberAnimation on x {
            duration: 900
            easing.type: Easing.InOutQuad
            id: animation
            running: false
        }
        Text {
            text: "连接服务器失败"
            font.bold: true
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Timer {
        id: timer
        interval: 1300
        onTriggered: {
            animation.to = root.width + 25
            animation.start()
        }
    }
}
