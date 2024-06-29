import QtQuick
import QtQuick.Window
import QtQuick.Controls


Item {
    height: 400
    visible: true
    width: 600
    //绘制电池体
    Rectangle {
        id: batteryBorder
        anchors.centerIn: parent
        border.color: "gray"
        border.width: 2
        color: "gray"
        height: 100
        radius: 10
        width: 200
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (checkBox.checkState === Qt.Checked) {
                    animation.start();
                }
            }
        }
    }

    Rectangle {
        anchors.left: batteryBorder.right
        anchors.verticalCenter: batteryBorder.verticalCenter
        border.color: "gray"
        border.width: 2
        color: "gray"
        height: 20
        radius: 3
        width: 5

    }
    //绘制电池块
    Rectangle {
        id: inRect
        property real level: 0.0 //position
        property real margin: 7
        property int inRectWidth: batteryBorder.width - 2 * margin
        anchors.left: batteryBorder.left
        anchors.leftMargin: margin
        anchors.verticalCenter: batteryBorder.verticalCenter
        color: "lightgreen"
        height: batteryBorder.height - 2 * margin
        radius: 10
        width: (batteryBorder.width - 2 * margin) * level
    }

    Text {
        property string currentBatteryValue: Math.floor(inRect.level * 100)
        anchors.centerIn: batteryBorder
        font.bold: true
        text: currentBatteryValue + "%"
        onCurrentBatteryValueChanged: {
            if (Number(currentBatteryValue) <= 20) {
                inRect.color = "red"
            } else if (Number(currentBatteryValue) > 20 && Number(currentBatteryValue) < 50) {
                inRect.color = "orange"
            } else {
                inRect.color = "lightgreen"
            }
        }
    }
    PropertyAnimation {
        id: animation
        target: inRect
        properties: "level"
        to: myslider.currentValue
        duration: 1000
        easing.type: Easing.InOutQuad
    }
    MySlider {
        width: 200
        id: myslider
        anchors.top: inRect.bottom
        anchors.horizontalCenter: batteryBorder.horizontalCenter
        anchors.topMargin: 20
        onValueChanged: {
            if (checkBox.checkState === Qt.Unchecked) {
                inRect.level = myslider.currentValue
                if (inRect.level < 0) {
                    inRect.level = 0
                }
                if (inRect.level > 1) {
                    inRect.level = 1
                }
            }
        }
    }
    CheckBox {
        id: checkBox
        text: "开启动画"
        anchors.left: myslider.right
        anchors.leftMargin: 30
        anchors.verticalCenter: myslider.verticalCenter
        checked: true
    }
}