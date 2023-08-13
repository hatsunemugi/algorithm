import QtQuick
import QtQml.Models
import "../layout"

DelegateModel {
    id: root
    property int width: 0
    signal select(var value)
    model: ListModel {
        ListElement{name: "持久化"}
        ListElement{name: "逆序"}
        ListElement{name: "拓扑排序"}
        ListElement{name: "广度优先"}
        ListElement{name: "深度优先"}
        ListElement{name: "prim"}
        ListElement{name: "kruskal"}
        ListElement{name: "floyd"}
        ListElement{name: "dijkstra"}
    }
    delegate: Rectangle {
        width: text.contentWidth
        height: text.contentHeight
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: text
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: name
        }
        MouseArea_ {
            onClicked: {
                select(index)
            }
        }
        Component.onCompleted: {
            if(width > root.width){
                root.width = width
            }
        }
    }
}


