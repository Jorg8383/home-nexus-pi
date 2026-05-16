import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: "Greenhouse Climate"
    background: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsL

        Item {
            Layout.fillHeight: true
        }

        ClimateSensorOverview {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            temperature: 32
            humidity: 70
            pressure: 1203
            airQuality: 175
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
