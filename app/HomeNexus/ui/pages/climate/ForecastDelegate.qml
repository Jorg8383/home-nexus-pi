import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper


Rectangle {
    id: root

    required property string dayText
    required property string timeText
    required property string weatherIcon
    required property real temperature
    required property real precipitationProbability

    property int padding: Style.appLayout.paddingS
    property color cardColor: Style.appColors.cardDelegateBackground
    property int cardRadius: Style.appRadius.radiusSmall

    radius: cardRadius
    color: cardColor

    implicitWidth: 140
    implicitHeight: contentLayout.implicitHeight + 2 * padding

    ColumnLayout {
        id: contentLayout

        // Position the ColumnLayout relative to its own delegate Rectangle
        x: root.padding
        y: root.padding
        width: Math.max(0, root.width - (2 * root.padding))

        spacing: Style.appLayout.spacingXS

        // Day
        Text {
            text: root.dayText
            color: Style.appColors.textBright
            font.pixelSize: Style.appTypography.fontSizeSubtitle
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight

            Layout.fillWidth: true
        }

        // Time
        Text {
            text: root.timeText
            color: Style.appColors.textBright
            font.pixelSize: Style.appTypography.fontSizeBody
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
        }

        // Weather icon
        Image {
            source: Style.assetsPath + WeatherIconMapper.fileName(root.weatherIcon)
            fillMode: Image.PreserveAspectFit
            asynchronous: true

            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 80
            Layout.preferredHeight: 80
        }

        // Temperature
        Text {
            text: Math.round(root.temperature) + "°C"
            color: Style.appColors.textBright
            font.pixelSize: Style.appTypography.fontSizeSubtitle
            font.bold: true
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
        }

        // Precipitation probability
        RowLayout {
             spacing: Style.appLayout.spacingS
             Layout.alignment: Qt.AlignHCenter

             Image {
                 source: Style.assetsPath + "weather/" + "drop-white-icon.svg"
                 fillMode: Image.PreserveAspectFit
                 Layout.preferredHeight: 14
                 Layout.preferredWidth: 14
             }

             Text {
                 text: Math.round(root.precipitationProbability * 100) + "%"
                 color: Style.appColors.textBright
                 font.pixelSize: Style.appTypography.fontSizeBody

             }
        }
    }
}
