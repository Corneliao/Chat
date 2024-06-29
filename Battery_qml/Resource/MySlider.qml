import QtQuick
import QtQuick.Window
import QtQuick.Controls

Rectangle {
    property var currentValue: 0

    signal valueChanged()

    id: root
    width: 200
    height: 10
    radius: 10
    Rectangle {
        id: control
        width: 200
        height: 10
        color: "gray"
        radius: 15
        Rectangle {
            property int sliderBodyWidth: 0
            id: sliderBody
            anchors.verticalCenter: control.verticalCenter
            width: sliderBodyWidth
            height: 10
            color: "lightgreen"
            radius: 15
        }
        Rectangle {
            property bool  isDraging: false
            property int dragX: 0
            id: slider
            width: 15
            height: 15
            color: "pink"
            radius: 15
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    slider.isDraging = true
                    slider.dragX = mouseX
                }
                onReleased: {
                    slider.isDraging = false
                }
                onPositionChanged: {
                    sliderBody.sliderBodyWidth = slider.x + slider.width - 5
                    if (slider.isDraging) {
                        slider.x += mouseX - slider.dragX
                        if (slider.x < 0) {
                            slider.x = 0;
                        }
                        if (slider.x > control.width - slider.width) {
                            slider.x = control.width - slider.width
                        }
                        root.currentValue = slider.x / (control.width - slider.width)
                        root.valueChanged();
                    }
                }
            }
        }
    }
}