import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: qsTr("Weather Settings")
    background: null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsXL
        spacing: Style.appLayout.spacingM

        AppInputTextCard {
            Layout.fillWidth: true

            labelText: qsTr("Weather location")

            debounceIntervalMs: 1500

            onSubmitted: function(cityName) {
                weatherViewModel.updateWeatherForCity(cityName, "", 5)
                console.log("Weather location updated by user:", cityName)
            }
        }

        // Invisble spacer item to push the visible elements to the top
        Item {
            Layout.fillHeight: true
        }
    }
}
