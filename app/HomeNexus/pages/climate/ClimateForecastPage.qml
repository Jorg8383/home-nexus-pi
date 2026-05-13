import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper



Page {
    id: root
    title: qsTr("Weather")
    background: null

    ColumnLayout {
        id: weatherApps
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsL

        RowLayout {

            spacing: Style.appLayout.spacingL

            // Current weather and location
            WeatherAppCard {
                id: currentWeather
                Layout.preferredHeight: 100
                Layout.fillWidth: true
                Layout.maximumWidth: weatherApps.width / 3
            }

            // Current wind
            AppCard {
                id: wind
                Layout.preferredHeight: 100
                Layout.fillWidth: true
            }

            // Current sunrise and sunset
            AppCard {
                id: sunrise
                Layout.preferredHeight: 100
                Layout.fillWidth: true
            }
        }

        // 5-day weather forecast in three hour intervals
        AppCard {
            id: forecastCard

            Layout.preferredHeight: 230
            Layout.fillWidth: true

            ListView {
                id: forecastListView

                anchors.fill: parent

                orientation: ListView.Horizontal
                spacing: Style.appLayout.spacingM
                clip: true

                model: StaticForecastModel {}

                delegate: ForecastDelegate {
                    height: forecastCard.height - 2 * Style.appLayout.paddingS
                }
            }
        }

    }
}
