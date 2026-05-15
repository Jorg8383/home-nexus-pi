import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus


AppCard {
    id: root

    property int sunrise: 1778817007 // Epoch Unix Timestamp
    property int sunset: 1778870707 // Epoch Unix Timestamp

    readonly property int iconSize: 32 // pixel

    function timeFromUnixTimestamp(timestamp) {
        const date = new Date(timestamp * 1000);
        return Qt.formatTime(date, "hh:mm")
    }

    padding: Style.appLayout.paddingXS

    ColumnLayout {
        anchors.fill: parent
        spacing: Style.appLayout.spacingXS

        // Sunrise
        RowLayout {
            spacing: Style.appLayout.spacingM
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Style.appLayout.marginsL

            Image {
                source: Style.assetsPath + "weather/sunrise50x50.png"
                fillMode: Image.PreserveAspectFit
                Layout.preferredHeight: root.iconSize
                Layout.preferredWidth: root.iconSize
            }

            Text {
                text: timeFromUnixTimestamp(root.sunrise)
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeBody
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
                Layout.leftMargin: Style.appLayout.marginsM
            }
        }

        // Sunset
        RowLayout {
            spacing: Style.appLayout.spacingM
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Style.appLayout.marginsL

            Image {
                source: Style.assetsPath + "weather/sunset50x50.png"
                fillMode: Image.PreserveAspectFit
                Layout.preferredHeight: root.iconSize
                Layout.preferredWidth: root.iconSize
            }

            Text {
                text: timeFromUnixTimestamp(root.sunset)
                color: Style.appColors.textBright
                font.pixelSize: Style.appTypography.fontSizeBody
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true
                Layout.leftMargin: Style.appLayout.marginsM
            }
        }
    }
}
