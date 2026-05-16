import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

import "../../utils/ClimateIconMapper.js" as ClimateIconMapper
import "../../utils/WeatherIconMapper.js" as WeatherIconMapper
import "../../utils/AirQualityMapper.js" as AirQualityMapper

Page {
    id: root
    title: qsTr("Home")
    background: null

    property real temperatureIndoor: 22.0
    property real temperatureOutdoor: 25.0
    property real temperatureGreenhouse: 30.0
    property real airQualityIndoor: 60.0
    property real airQualityGreenhouse: 210.0
    property real humidityIndoor: 40.939
    property real humidityGreenhouse: 68.34
    property real windSpeed: 7.889
    property string weatherIcon: "10d"


    Flickable {
        id: flickable

        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsXL

        clip: true

        contentWidth: flickable.width
        contentHeight: homeGrid.height

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds

        Item {
            id: contentWrapper

            width: flickable.width
            height: homeGrid.implicitHeight

            GridLayout {
                id: homeGrid

                readonly property int preferredCardWidth: 300

                anchors.horizontalCenter: parent.horizontalCenter

                columns: 2
                rowSpacing: Style.appLayout.spacingL
                columnSpacing: Style.appLayout.spacingL


                AppMetricCard {
                    title: qsTr("Indoor Temperature")
                    value: root.temperatureIndoor.toFixed(1)
                    unit: "°C"
                    iconSource: Style.assetsPath + ClimateIconMapper.temperatureIcon(root.temperatureIndoor)

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Greenhouse Temperature")
                    value: root.temperatureGreenhouse.toFixed(1)
                    unit: "°C"
                    iconSource: Style.assetsPath + ClimateIconMapper.temperatureIcon(root.temperatureGreenhouse)

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Indoor Air Quality")
                    value: AirQualityMapper.iaqText(root.airQualityIndoor)
                    unit: ""
                    iconSource: Style.assetsPath + ClimateIconMapper.airQualityIcon(root.airQualityIndoor)

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Greenhouse Air Quality")
                    value: AirQualityMapper.iaqText(root.airQualityGreenhouse)
                    unit: ""
                    iconSource: Style.assetsPath + ClimateIconMapper.airQualityIcon(root.airQualityGreenhouse)

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Indoor Humidity")
                    value: root.humidityIndoor.toFixed(1)
                    unit: "%"
                    iconSource: Style.assetsPath + "climate/humidity-white.svg"

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Greenhouse Humidity")
                    value: root.humidityGreenhouse.toFixed(1)
                    unit: "%"
                    iconSource: Style.assetsPath + "climate/humidity-white.svg"

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }

                AppMetricCard {
                    title: qsTr("Weather")
                    value: root.temperatureOutdoor.toFixed(1)
                    unit: "°C"
                    iconSource: Style.assetsPath + WeatherIconMapper.fileName(root.weatherIcon)

                    Layout.preferredWidth: homeGrid.preferredCardWidth
                }
            }
        }
    }
}
