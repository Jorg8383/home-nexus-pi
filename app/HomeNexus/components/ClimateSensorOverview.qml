import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

import "../utils/ClimateIconMapper.js" as ClimateIconMapper

GridLayout {
    id: root

    property real temperature: 0.0
    property real humidity: 0.0
    property real pressure: 0.0
    property real airQuality: 0.0

    property string temperatureTitle: qsTr("Temperature")
    property string humidityTitle: qsTr("Humidity")
    property string pressureTitle: qsTr("Air pressure")
    property string airQualityTitle: qsTr("Air quality")

    columns: 2
    rowSpacing: Style.appLayout.spacingXL
    columnSpacing: Style.appLayout.spacingXL

    readonly property int preferredCardWidth: 300

    ClimateMetricCard {
        title: root.temperatureTitle
        value: root.temperature.toFixed(1)
        unit: "°C"
        iconSource: Style.assetsPath + ClimateIconMapper.temperatureIcon(root.temperature)

        Layout.preferredWidth: root.preferredCardWidth
    }

    ClimateMetricCard {
        title: root.humidityTitle
        value: root.humidity.toFixed(0)
        unit: "%"
        iconSource: Style.assetsPath + "climate/humidity-white.svg"

        Layout.preferredWidth: root.preferredCardWidth
    }

    ClimateMetricCard {
        title: root.airQualityTitle
        value: root.airQuality.toFixed(0)
        unit: "IAQ"
        iconSource: Style.assetsPath + ClimateIconMapper.airQualityIcon(root.airQuality)

        Layout.preferredWidth: root.preferredCardWidth
    }

    ClimateMetricCard {
        title: root.pressureTitle
        value: root.pressure.toFixed(0)
        unit: "hPa"
        iconSource: Style.assetsPath + "climate/pressure-gauge-white.svg"

        Layout.preferredWidth: root.preferredCardWidth
    }
}
