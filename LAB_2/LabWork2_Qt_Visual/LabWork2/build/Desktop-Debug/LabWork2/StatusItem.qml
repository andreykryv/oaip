// StatusItem.qml
import QtQuick 2.15
import QtQuick.Layouts 1.15

RowLayout {
    property string label: ""
    property string value: ""
    spacing: 4
    Text { text: label + ":"; color: "#a8c0dc"; font.pixelSize: 11; font.family: "Arial" }
    Text { text: value;       color: "#ffffff"; font.pixelSize: 11; font.family: "Arial"; font.weight: Font.Medium }
}
