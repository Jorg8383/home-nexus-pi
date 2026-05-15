import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper

AppCard {
    id: root

    property string location: "Mehrstetten"
    property string description: "shower rain"
    property string icon: "10d"
    property real temperature: 8.3

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
                text: root.location
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeTitle
                font.bold: true
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight

                Layout.fillWidth: true
            }

            // Current temperature
            Text {
                text: Math.round(root.temperature) + " °C"
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeBody
                horizontalAlignment: Text.AlignLeft

                Layout.fillWidth: true
            }

            // Current weather description
            Text {
                text: qsTr(root.description)
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
            source: Style.assetsPath + "weather/" + WeatherIconMapper.fileName(root.icon)
            fillMode: Image.PreserveAspectFit

            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: 80
            Layout.preferredHeight: 80
        }
    }
}
