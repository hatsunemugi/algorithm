import QtQml
import QtQuick
import QtQuick.Controls
import algorithm
Item {
    id: root
    property QtObject source
    property QtObject target
    property Edge edge/*: Edge{}*/
    property alias text: weight
    property bool reverse: false
    property point point
    property int size: 8
    signal edit(QtObject object)

    anchors.centerIn: source
    rotation: angle()
//    Edge {
//        id: edge
//        onReversed: {
//            if(arrow.state === "left")
//                arrow.state = "right"
//            else if(arrow.state === "right")
//                arrow.state = "left"
//        }
//        onVisited: {
//            area.base_color = "#ee7c2d"
//        }
//        onLeft: {
//            area.base_color = "#66000000"
//        }
//    }

    Rectangle {
        id: rect
        z: parent.parent.z
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        radius: height/2
        width: length()
        height: size
        Rectangle {
            anchors.centerIn: parent
            width: weight.contentWidth
            height: weight.contentHeight
            rotation: -root.rotation
            color: "transparent"
            TextInput {
                id: weight
                font.pointSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.centerIn: parent
                onFocusChanged: {
                    if(!focus){
                        if(text === "")
                            text = "0"
                        if(edge != null)
                            edge.set(text)
                    }
                    else{
                        if(target == null)
                            root.destroy()
                        if(text === "0")
                            text = ""
                        root.edit(root)
                    }
                }
            }
        }
        Image {
            id: arrow
            sourceSize: Qt.size(32, 32)
            anchors.verticalCenter:parent.verticalCenter
            source: "../image/black/return.svg"
            state: "right"
            states: [
                State {
                    name: "left"
                    PropertyChanges {
                        target: arrow
                        rotation: 0
                        x: -12
                    }
                },
                State {
                    name: "right"
                    PropertyChanges {
                        target: arrow
                        rotation: 180
                        x: parent.width - 20
                    }
                }
            ]
        }
        MouseArea_ {
            id: area
            base_color: "#50000000"
            hover_color: "#25000000"
            press_color: "#99000000"
            hoverEnabled: false
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: (mouse)=>{
                if(mouse.button === Qt.RightButton)
                {
                    weight.focus = !weight.focus
                }
                else
                {
                    console.log(edge.toString())
                }
            }
        }
    }
    function length() {
        if(source == null)
            return 0
        var value
        if(target != null){
            value = Math.sqrt(Math.pow(target.x - source.x, 2) + Math.pow(target.y - source.y, 2))
        }
        else
        {
            value = Math.sqrt(Math.pow(point.x - source.x, 2) + Math.pow(point.y - source.y, 2))
        }

        return value - 26
    }
    function angle() {
        if(source == null)
            return 0
        var dx
        var dy
        if(target != null)
        {
            dx = target.x - source.x
            dy = target.y - source.y
        }
        else
        {
            dx = point.x - source.x
            dy = point.y - source.y
        }

        if(dy === 0)
            return dx>=0 ? 0 : 180
        if(dx === 0)
            return dy>=0 ? 90 : 270

        var value = Math.atan(Math.abs(dx) / Math.abs(dy)) * 180 / Math.PI

        if(dx > 0 && dy > 0){
            return 90 - value
        }
        if(dx < 0 && dy > 0){
            return value + 90
        }
        if(dx < 0 && dy < 0){
            return 270 - value
        }
        if(dx > 0 && dy < 0){
            return value + 270
        }
        return value
    }
    function connection(object) {
        edge = object
        edge.visited.connect(onVisited)
        edge.left.connect(onLeft)
        edge.reversed.connect(onReversed)
    }
    function onReversed() {
        if(arrow.state === "left")
            arrow.state = "right"
        else if(arrow.state === "right")
            arrow.state = "left"
    }
    function onVisited() {
        if(area.base_color != "#ee7c2d")
            area.base_color = "#ee7c2d"
        else
            area.base_color = "#ffa07a"
    }
    function onLeft() {
        area.base_color = "#66000000"
    }
    onSourceChanged: {
        point = Qt.point(source.x,source.y)
    }

    onTargetChanged: {
//        console.log(z,target.z)
        area.hoverEnabled = true
        graph.set(source.vertex,target.vertex,edge)
    }
}
