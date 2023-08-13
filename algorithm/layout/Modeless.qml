import QtQuick 2.15
import QtQuick.Controls 2.15
import algorithm 1.0
Popup {
    id: root
    margins: 0
    padding: 0
    modal: true
    focus: true
    dim:false

//    Overlay.modeless: Rectangle {
//        color: "transparent"
//    }
//    background: Rectangle {
//        color: "transparent"
//    }
    closePolicy:Popup.CloseOnPressOutside
    Component.onCompleted: {close()}
    Component.onDestruction: console.log(x,y)
}

