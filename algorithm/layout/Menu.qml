import QtCore
import QtQuick
import algorithm
import QtQuick.Dialogs
import QtQuick.Controls
import "../values"
Popup {
    id: root
    modal: true
    dim:false
    closePolicy:Popup.CloseOnPressOutside
    function openAt(x,y) {
        open()
        root.x = x
        root.y = y
    }
    Rectangle {
        id: drager
        height: 24
        x: parent.x - width
        y: parent.y - height
        width: menuModel.width + 8
        MouseArea_ {
            base_color: "transparent"
            onPressed: (mouse)=>{
               drag.target = parent
               hoverEnabled = false

            }
            onReleased: (mouse)=>{
                drag.target = null
                hoverEnabled = true
            }
        }
        Rectangle {
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: menuModel.count * menuModel.height + 4
            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 2
                Repeater {
                    model: menuModel
                }
            }
        }
    }





    FileDialog {
        id:loader
        title: "选择文件"
        currentFolder:StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
        fileMode: FileDialog.OpenFiles
        nameFilters: ["所有文件 (*.*)"]
        onAccepted: {
            graph.load(selectedFile)
        }
        onRejected: {
            console.log("Canceled");
        }

    }

    FileDialog {
        id:saver
        title: "选择文件"
        currentFolder:StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
        fileMode: FileDialog.SaveFile
        nameFilters: ["所有文件 (*.*)"]
        onAccepted: {
            graph.save(selectedFile)
        }
        onRejected: {
            console.log("Canceled");
        }

    }

    MenuModel{
        id: menuModel
        onSelect: (value)=>{
            switch(value)
            {
                case 0: loader.open();break;
                case 1: saver.open();break;
                case 2: graph.clear();break;
                default:
                    break;
            }
            root.close()
        }
    }
    background: Rectangle {
        color: "transparent"
    }
}


