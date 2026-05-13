import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper



Page {
    id: root
    title: "Weather Forecast"
    background: null

    ColumnLayout {
        id: weatherApps
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsL

        RowLayout {

            spacing: Style.appLayout.spacingL

            // Current weather and location
            AppCard {
                id: currentWeather
                Layout.preferredHeight: 100
                Layout.fillWidth: true
                Layout.maximumWidth: weatherApps.width / 3
                padding: Style.appLayout.paddingXS

                RowLayout {
                    anchors.fill: parent
                    spacing: Style.appLayout.spacingM

                    ColumnLayout {
                        spacing: Style.appLayout.spacingXS
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: Style.appLayout.marginsL

                        // Location
                        Text {
                            text: qsTr("Mehrstetten")
                            color: Style.appColors.textBright
                            font.pixelSize: Style.appTypography.fontSizeTitle
                            font.bold: true
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight

                            Layout.fillWidth: true
                        }

                        // Current temperature
                        Text {
                            text: qsTr("8°C")
                            color: Style.appColors.textBright
                            font.pixelSize: Style.appTypography.fontSizeBody
                            horizontalAlignment: Text.AlignLeft

                            Layout.fillWidth: true
                        }

                        // Current weather description
                        Text {
                            text: qsTr("shower rain")
                            color: Style.appColors.textBright
                            font.pixelSize: Style.appTypography.fontSizeBody
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight

                            Layout.fillWidth: true
                        }
                    }

                    // Weather icon
                    Image {
                        id: weatherIcon
                        source: Style.assetsPath + "weather/" + WeatherIconMapper.fileName("10d")
                        fillMode: Image.PreserveAspectFit

                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 80
                        Layout.preferredHeight: 80
                    }
                }
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
