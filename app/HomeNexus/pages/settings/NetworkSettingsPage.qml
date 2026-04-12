import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Network Settings"
    background: null

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Network Settings Details"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Placeholder WiFi settings"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
