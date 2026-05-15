import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus


AppCard {
    id: root

    // Values later provided by the C++ backend
    property real windSpeed: 3.98
    property real windDirDegrees: 225

    padding: Style.appLayout.paddingXS

    function windDirectionText(degrees) {
        const directions = [
                         qsTr("North"),
                         qsTr("North-east"),
                         qsTr("East"),
                         qsTr("South-east"),
                         qsTr("South"),
                         qsTr("South-west"),
                         qsTr("West"),
                         qsTr("North-west"),
                         ]

        const normalized = ((degrees % 360) + 360) % 360
        const index = Math.round(normalized / 45.0) % 8
        return directions[index]
    }

    RowLayout {
        anchors.fill: parent
        spacing: Style.appLayout.spacingM

        ColumnLayout {
            spacing: Style.appLayout.spacingXS
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Style.appLayout.marginsL

            // Headline
            Text {
                text: qsTr("Wind")
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeTitle
                font.bold: true
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight

                Layout.fillWidth: true
            }

            // Wind direction
            Text {
                text: root.windDirectionText(root.windDirDegrees)
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeBody
                horizontalAlignment: Text.AlignLeft

                Layout.fillWidth: true
            }

            // Wind speed
            Text {
                text: Number(root.windSpeed).toFixed(1) + " m/s"
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeBody
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight

                Layout.fillWidth: true
            }
        }

        // Wind direction arrow

        Image {
            id: weatherIcon
            source: Style.assetsPath + "weather/wind-arrow-white.svg"
            fillMode: Image.PreserveAspectFit

            Layout.rightMargin: Style.appLayout.marginsL
            Layout.preferredWidth: 52
            Layout.preferredHeight: 52

            // Arrow points to where the wind blows
            rotation: root.windDirDegrees + 180

            Behavior on rotation {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
