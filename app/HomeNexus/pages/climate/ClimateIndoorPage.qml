import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Indoor Climate"
    background: null

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: "Indoor Climate"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Temperature: --.- °C"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Humidity: -- %"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Air Quality: --"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Later connected to Pi sensor values"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
