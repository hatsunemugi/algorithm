import QtQuick 2.15
import QtQuick.Controls 2.15

Modeless {
    id: root
    property alias text: text
    property alias color: rect.color
    property alias interval: timer.interval
    width: text.contentWidth + 8
    height: text.contentHeight + 4
    signal timeout(QtObject object)
    x: 0
    y: 0
    function toast(time) {
        if(timer.running && timer.interval>time)
            return
        if(time > 0)
            if(timer.interval !== time)
                timer.interval = time
        timer.running = true
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "transparent"
        border.color: "black"
        border.width: 1
        Text {
            id: text
            text: "null"
            anchors.centerIn: parent
            font.pointSize: 8
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
        id: timer
        interval: Qt.styleHints.mousePressAndHoldInterval
        running: false
        repeat: false
        onTriggered: {
            root.timeout(root)
        }
    }

    background: Rectangle {
        color: "transparent"
    }
    closePolicy:Popup.NoAutoClose
    Component.onCompleted: {

    }
}
