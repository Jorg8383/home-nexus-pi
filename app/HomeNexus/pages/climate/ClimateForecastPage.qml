import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

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
            anchors.centerIn: parent
            source: Style.assetsPath + "weather/clear-day.svg"
            width: 200
            height: 200
        }
    }

}
