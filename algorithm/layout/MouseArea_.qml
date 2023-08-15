import QtQuick
import QtQuick.Controls
MouseArea {
    id: root
    property color base_color: '#ffffff'
    property color base_color_light: '#ffffff'
    property color base_color_dark: '#f5f5f5'
    property color hover_color: '#f0f0f0'
    property color press_color: '#d0d0d0'
    anchors.fill: parent
    hoverEnabled: true
    onHoveredChanged: {
        parent.color= containsMouse ?  hover_color :base_color
    }
    onEntered: {
        toast.toast()
    }
    onPressed: {
        parent.color= press_color
    }
    onReleased: {
        parent.color= containsMouse ? hover_color :base_color
        console.log("clicked")
    }
    onBase_colorChanged: {
        if(parent != null)
        parent.color = base_color
    }
    Component.onCompleted: parent.color = base_color
    ToolTip{
        id: tip
        text: root.parent.color
        delay: Qt.styleHints.mousePressAndHoldInterval
    }
    Toast {
        id: toast
        interval: 1000
        x: mouseX
        y: root.y + height
        onTimeout: {
            text.text = root.parent.color
            visible = root.containsMouse
            if(visible)
                toast.toast()
        }
    }
}
