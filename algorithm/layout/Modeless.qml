import QtQuick 2.15
import QtQuick.Controls 2.15
Popup {
    id: root
    margins: 0
    padding: 0
    modal: false
    focus: true
    dim:false

    background: Rectangle {
        color: "transparent"
    }
    closePolicy:Popup.CloseOnPressOutside
}

