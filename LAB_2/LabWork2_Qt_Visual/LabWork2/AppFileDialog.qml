import QtQuick 2.15
import QtQuick.Controls 2.15

QtObject {
    id: root
    property string mode: "open"
    property string selectedFile: ""
    property string title: mode === "open" ? "Открыть файл" : "Сохранить файл"

    signal accepted()
    signal rejected()

    function open() {
        fileDialogHelper.requestDialog(mode, title)
    }

    property var _conn: Connections {
        target: fileDialogHelper
        function onFileSelected(path) {
            root.selectedFile = path
            root.accepted()
        }
        function onDialogRejected() {
            root.rejected()
        }
    }
}