import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Weather Forecast"
    background: null

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: "Forecast"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Today: --"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Tomorrow: --"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Next days: --"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Later fetched from OpenWeather API"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
