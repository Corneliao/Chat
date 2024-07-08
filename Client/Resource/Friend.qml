import QtQuick
import QtQuick.Controls

Rectangle {
    width: parent.width
    height: parent.height
    color: "#f0f0f0"
    radius: 20
    Rectangle {
        id: rect
        width: 600
        height: 20
        radius: 15
        color: "white"
        anchors.centerIn: parent
        Rectangle {
            parent: parent
            id: slider
            width: 100
            height: parent.height
            radius: 15
            color: "orange"
            onXChanged: {

                if (x === rect.width - 100) {
                    widthAnimation.to = 0
                    slider.anchors.right = parent.right
                    //  widthAnimation.start()
                }
            }
            onWidthChanged: {

                if (width === 0) {
                    widthAnimation.to = 100
                }
                if (width === 100) {

                }
            }
        }
    }

    SequentialAnimation {
        running: true
        loops: Animation.Infinite

        PropertyAnimation {
            id: moveAnimation
            target: slider
            property: "x"
            to: rect.width - slider.width
            duration: 1000
            //  loops: Animation.Infinite
        }
        PropertyAnimation {
            id: widthAnimation
            target: slider
            property: "width"
            to: 100
            duration: 1000
        }
    }

    // ParallelAnimation {
    //     id: moveAndWidth
    //     running: true
    //     loops: Animation.Infinite
    //     PropertyAnimation {
    //         id: moveAnimation
    //         target: slider
    //         property: "x"
    //         to: rect.width - slider.width
    //         duration: 1000
    //     }
    //     PropertyAnimation {
    //         id: widthAnimation
    //         target: slider
    //         property: "width"
    //         to: 150
    //         duration: 1000
    //     }
    // }
}
