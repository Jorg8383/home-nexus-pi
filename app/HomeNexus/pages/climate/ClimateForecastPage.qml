import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper



Page {
    id: root
    title: "Weather Forecast"
    background: null

    AppCard {
        id: forecastCard

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.leftMargin: Style.appLayout.marginsL
        anchors.rightMargin : Style.appLayout.marginsL
        anchors.bottomMargin: Style.appLayout.marginsL

        height: 230

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
