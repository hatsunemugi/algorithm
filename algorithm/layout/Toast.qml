import QtQuick 2.15
import QtQuick.Controls 2.15

Popup {
    id: root
    property alias text: text
    property alias color: rect.color
    width: text.contentWidth + 8
    height: text.contentHeight + 4
    signal timeout(QtObject object)
    x: 0
    y: 0
    margins: 0
    padding: 0
    modal: false
    focus: true
    dim:true
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: "black"
        border.width: 2
        Text {
            id: text
            text: "null"
            anchors.centerIn: parent
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                drag.target = parent
            }
        }
    }
    Timer {
        interval: 2000
        running: true
        repeat: false
        onTriggered: {
            root.timeout(root)
        }
       }

    Overlay.modeless: Rectangle {
        color: "transparent"
    }
    background: Rectangle {
        color: "transparent"
    }
    closePolicy:Popup.NoAutoClose
    Component.onCompleted: {
        close()
    }
    Component.onDestruction: console.log(x,y)
}
