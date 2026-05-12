import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper

Page {
    id: root
    title: "Weather Forecast"
    background: null

    Rectangle {
        x: 30
        y: 80
        width: parent.width - 60
        height: parent.height - 110
        color: Style.appColors.cardBackground

        Image {
            id: weatherIcon
            anchors.centerIn: parent
            source: Style.assetsPath + "weather/" + WeatherIconMapper.fileName("01d")
            width: 200
            height: 200
        }
    }

}
