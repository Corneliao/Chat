import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    width: width
    height: height
    color: "#f0f0f0"
    ToolsBar {
        objectName: "toolbar"
        id: toolbar
        width: 200
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 5
        onSetStackeLayoutIndex: {
            stackedLayout.currentIndex = toolbar.listviewCurrentIndex
        }
    }

    StackLayout {
        objectName: "stackLayout"
        id: stackedLayout
        width: parent.width - toolbar.width - 30
        height: parent.height - 20
        anchors.left: toolbar.right
        currentIndex: 0
        UserLogin {
            objectName: "userlogin"
            id: userlogin
            width: parent.width
            height: parent.height
            onShowContainer: {
                stackedLayout.currentIndex = 1
                toolbar.updateListviewIndex(1)
            }
            onUserdisconnection: {

            }
        }
        Content {
            objectName: "container"
            id: container
            width: parent.width
            height: parent.height
            onConnectionState: {
                if (userlogin.isConnectioned) {
                    container.disConnection()
                } else {
                    console.log("未连接服务器")
                }
            }
        }
        Friend {}
    }
    // GaussianBlur {
    //     anchors.fill: stackedLayout
    //     source: stackedLayout
    //     radius: 8
    //     samples: 8
    // }
}
