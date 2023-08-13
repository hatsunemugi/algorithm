import QtQuick 2.15
import algorithm
MouseArea {
    id: root
    anchors.fill: parent
    property Node node
    property QtObject source
    property QtObject target
    property QtObject line
    property QtObject edit
    property var map
    property int count:1
    hoverEnabled: true
    signal select(QtObject object)
    signal menu(int x,int y)
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onClicked: (mouse)=>{

        if(mouse.button === Qt.RightButton){
            menu(mouseX,mouseY)
            return
        }
        if(line != null)
        {
            line.destroy()
            return
        }
        if(edit != null)
        {
            edit.text.focus = !edit.text.focus
            edit = null
            return
        }

//            var component = Qt.createComponent("Toast.qml");
        functions.createVertex()
    }
    Item {
        id: functions
        function createVertex(){
            var vertex = Qt.createComponent("Ellipse.qml").createObject(root)
            vertex.select.connect(functions.onSelect)
            vertex.line.connect(functions.onLine)
            vertex.edit.connect(functions.onEdit)
            vertex.x = mouseX
            vertex.y = mouseY
            vertex.connection(graph.get("Vertex"))
            vertex.vertex.set(count)
            vertex.text.text = vertex.vertex.get()
            count++
        }
        function createEdge(object) {
            line = Qt.createComponent("Line.qml").createObject(root)
            line.edit.connect(onEdit)
            line.source = object
            line.connection(graph.get("Edge"))
            line.text.text = line.edge.get()
        }

        function createVertexFrom(object){
            var vertex = Qt.createComponent("Ellipse.qml").createObject(root)
            vertex.select.connect(functions.onSelect)
            vertex.line.connect(functions.onLine)
            vertex.edit.connect(functions.onEdit)
            vertex.x = Math.floor(Math.random() * root.width * 0.5) + root.width * 0.25
            vertex.y = Math.floor(Math.random() * root.height * 0.5) + root.height * 0.25
            vertex.connection(object)
            vertex.text.text = object.get()
            count++
            return vertex
        }
        function createEdgeFrom(source,target,edge) {
            line = Qt.createComponent("Line.qml").createObject(root)
            line.edit.connect(onEdit)
            line.source = source
            line.connection(edge)
            line.text.text = line.edge.get()
            line.target = target
            line = null
        }

        function onSelect(object){
            if(source != null)
                target = object
            else{
                root.select(object)
                graph.set(object.vertex)
            }
            if(line != null)
            {
                if(source != target)
                {
                    line.target = target
                    line = null
                    source = null
                    target = null
                }
            }
        }

        function onLine(object) {
            console.log("select",object.x,object.y)
            source = object
            if(line == null)
            {
//                line = Qt.createComponent("Line.qml").createObject(root)
//                line.edit.connect(onEdit)
//                line.source = object
                createEdge(object)
            }
        }
        function onEdit(object) {
            edit = object
        }
    }



    onMouseXChanged: {
        if(line != null)
        {
            line.point = Qt.point(mouseX,mouseY)
        }
    }
    onMouseYChanged: {
        if(line != null)
        {
            line.point = Qt.point(mouseX,mouseY)
        }
    }
    Component.onCompleted: {
        map = new Map();
    }

    Connections {
        target: graph
        function onPaint(object) {
            node = object
            console.log(node.type == 0? "Edge" : "Vertex",node.toString())
            if(node.type === Node.Type.Vertex)
            {

                map.set(node.toString(),functions.createVertexFrom(object))

            }
            if(node.type === Node.Type.Edge)
            {
                var a = node.toString().split("->")
//                for(let s of a)
                var source = map.get(a[0])
                var target = map.get(a[2])
                functions.createEdgeFrom(source,target,object)
//                line.target = target
            }
        }
    }
}
