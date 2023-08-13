import QtQuick
import QtQuick.Window

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
//    color: "red"
    GraphBuilder {
        onMenu: (x,y)=>{
            loader.openAt(x,y)
        }
    }
    Panel {}
    Menu {
        id: loader
    }
}
