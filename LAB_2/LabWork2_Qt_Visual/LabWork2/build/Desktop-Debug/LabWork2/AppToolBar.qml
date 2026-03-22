// AppToolBar.qml
import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    implicitHeight: 48
    color: "#f8fafd"
    border.color: "#dce6f0"
    border.width: 1
    radius: 6

    default property alias items: row.data

    RowLayout {
        id: row
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 6
    }
}
