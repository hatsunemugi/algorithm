import QtQuick 2.15
import QtQuick.Controls 2.15
import algorithm
Item {
    id: root
    property int size: 24
    property Vertex vertex/*: Vertex {}*/
    property alias text: name
    property int count: 0
    signal select(QtObject object)
    signal line(QtObject object)
    signal edit(QtObject object)
    Rectangle {
        id: rect
        width: size
        height: size
        x: -size/2
        y: -size/2
        color: "transparent"
        radius: size/2
        border.color: "black"
        border.width: 2
        Rectangle {
            anchors.fill: parent
            anchors.margins: 2
            radius: (size-8)/2
            Rectangle {
                height: name.contentHeight + 4
                width: name.contentWidth + 8
                color: "transparent"
                anchors.left: parent.right
                anchors.bottom: parent.top
                border.color: "black"
                border.width: 1
                TextInput {
                    id: name
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    onFocusChanged: {
                        if(!focus){
                            root.vertex.set(text)
                        }
                        else{
                            root.edit(root)
                        }
                    }
                }
            }

            MouseArea_ {
                id: area
                base_color: "#9900ff00"
                hover_color: "#ff00bbbb"
                press_color: "#99ff0000"
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                propagateComposedEvents: true

                onPressed: (mouse)=>{
                    if (mouse.button === Qt.LeftButton)
                    {
                       drag.target = root
                       hoverEnabled = false
                    }
                    mouse.accepted = true;

                }
                onReleased: (mouse)=>{
                    if (mouse.button === Qt.LeftButton)
                    {
                        drag.target = null
                        hoverEnabled = true
                        vertex.setPosition(root.x,root.y);
                    }
                    mouse.accepted = true;
                }
                onClicked: (mouse)=>{
                    if (mouse.button === Qt.LeftButton)
                    {
                        select(root)

                    }
                    if(mouse.button === Qt.RightButton)
                    {
                       name.focus = !name.focus
                    }
                    mouse.accepted = true;
                }
                onDoubleClicked: (mouse)=>{
                    if (mouse.button === Qt.LeftButton)
                    {
                        line(root)
                    }
                    mouse.accepted = true;
                }
            }
        }
    }
    SequentialAnimation {
        id: wave
        NumberAnimation { target: root; property: "size"; to: 28; easing.type: Easing.InOutQuad; duration: 64; }
        NumberAnimation { target: root; property: "size"; to: 24; easing.type: Easing.InOutQuad; duration: 64; }
    }
    function connection(object) {
        vertex = object
        vertex.visited.connect(onVisited)
        vertex.left.connect(onLeft)
        vertex.position.connect(onPosition)
        vertex.deleted.connect(onDeleted)
    }
    function onVisited() {
        area.base_color = graph.color(count)
        count++
//        if(area.base_color != "#ee7c2d")
//            area.base_color = "#ee7c2d"
//        else
//            area.base_color = "#ffa07a"
        toast.toast(128)
        wave.running = true
    }
    function onLeft() {
        count = 0
        area.base_color = "#9900ff00"
    }
    function onPosition(x,y) {
        root.x = x
        root.y = y
        vertex.setPosition(x,y)
    }
    function onDeleted() {
        root.destroy();
    }
    Toast {
        id: toast
        y: parent.height
        text.text: count
        text.font.pointSize: 12
        onTimeout: {
            visible = !visible
            if(visible)
                toast.toast(5000)
        }
    }
}
