import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: "Indoor Climate"
    background: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsL

        Item {
            Layout.fillHeight: true
        }

        ClimateSensorOverview {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            temperature: 25
            humidity: 48
            pressure: 1013
            airQuality: 80
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
