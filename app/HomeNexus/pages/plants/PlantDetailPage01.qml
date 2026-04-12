import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Plant Details"
    background: null

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Plant Details"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Placeholder for plant-specific information"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
