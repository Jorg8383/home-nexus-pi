import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

import "../../utils/WeatherIconMapper.js" as WeatherIconMapper

AppCard {
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
