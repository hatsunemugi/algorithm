import QtQuick
import QtQml.Models
import "../layout"

DelegateModel {
    id: root
    property int width: 0
    property int height: 0
    signal select(var value)
    model: ListModel {
        ListElement{name: "打开"}
        ListElement{name: "保存"}
        ListElement{name: "清空"}
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
            root.height = text.contentHeight
        }
    }
}


