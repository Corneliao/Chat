import QtQuick.Controls
import QtQuick.Window
import QtQuick

Rectangle {
    property string headtitleName: "Flache"
    property  int listviewCurrentIndex: 0

    signal setStackeLayoutIndex();
    id: root
    visible: true
    radius: 10
    Rectangle {
        anchors.top: parent.top
        anchors.topMargin: 5
        radius: 10
        id: headtitle
        width: parent.width
        height: 80
        Text {
            text: root.headtitleName
            anchors.centerIn: parent
            font.bold: true
            color: "#fe8830"
            font.pixelSize: 30
        }
    }
    ListView {
        id: listview
        width: parent.width
        height: parent.height - headtitle.height - info.height
        anchors.top: headtitle.bottom
        anchors.topMargin: 40
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        model: ListModel {
            ListElement {
                itemName: "连接"
                iconPath: "qrc:/Resource/line.png"
            }
            ListElement {
                itemName: "消息"
                iconPath: "qrc:/Resource/line.png"
            }
            ListElement {
                itemName: "好友"
                iconPath: "qrc:/Resource/line.png"
            }
            ListElement {
                itemName: "动态"
                iconPath: "qrc:/Resource/line.png"
            }
        }
        delegate: Rectangle {
            id: listviewDelegate
            width: 160
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            radius: 10
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    listview.currentIndex = index
                    root.listviewCurrentIndex = index
                    root.setStackeLayoutIndex();
                }
                onEntered: {
                    listviewDelegate.color = "#f0f0f0"
                }
                onExited: {
                    listviewDelegate.color = "transparent"
                }
            }
            Text {
                id: itemname
                text: model.itemName
                font.bold: !!listviewDelegate.ListView.isCurrentItem
                color: listviewDelegate.ListView.isCurrentItem ? "#fe8830" : "black"
                font.pixelSize: listviewDelegate.ListView.isCurrentItem ? 16 : 14
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Image {
                width: 40
                height: itemname.height
                source: listviewDelegate.ListView.isCurrentItem ? model.iconPath : ""
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
    Rectangle {
        id: info
        width: parent.width
        height: 80
        anchors.bottom: parent.bottom
        anchors.topMargin: 20
        radius: 10
        Text {
            text: "当前版本V 1.0"
            font.bold: true
            anchors.centerIn: parent
            font.pixelSize: 14
            color: "#fe8830"
        }
    }

    function updateListviewIndex(index) {
        listview.currentIndex = index
    }
}
