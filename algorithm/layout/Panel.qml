import QtQuick
import algorithm
import "../values"

Rectangle {
    id: root
    property alias count:listview.count
    width: panelModel.width + 8
    height: listview.count * 32 + 48
    color: "transparent"
    border.color: "black"
    border.width: 2
    Rectangle {
        id: drager
        height: 24
        anchors.margins: 2
        anchors {top: parent.top; left: parent.left; right: parent.right}
        MouseArea_ {
            base_color: "transparent"
            onPressed: (mouse)=>{
               drag.target = root
               hoverEnabled = false

            }
            onReleased: (mouse)=>{
                drag.target = null
                hoverEnabled = true
            }
        }
    }

    ListView {
        id: listview
        anchors {top: drager.bottom; bottom:parent.bottom; left: parent.left; right: parent.right}
        model: panelModel
        spacing: 16
        clip: true
    }
    PanelModel{
        id: panelModel
        onSelect: (value)=>{
            graph.exec(value)
        }
    }

}

