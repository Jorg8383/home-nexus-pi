import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: qsTr("Settings")
    background: null

    required property StackView stackView

    Flickable {
        id: flickable

        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsXL

        clip: true

        contentWidth: flickable.width
        contentHeight: contentWrapper.height

        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds

        Item {
            id: contentWrapper

            width: flickable.width
            height: homeGrid.implicitHeight

            GridLayout {
                id: homeGrid

                readonly property int preferredCardWidth: 160
                readonly property int preferredIconSize: 75

                anchors.horizontalCenter: parent.horizontalCenter

                columns: 4
                rowSpacing: Style.appLayout.spacingXL
                columnSpacing: Style.appLayout.spacingXL

                AppTopicCard {
                    title: qsTr("Network")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/wifi-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsNetworkPage.qml")
                }

                AppTopicCard {
                    title: qsTr("System")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/settings-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsSystemPage.qml")
                }

                AppTopicCard {
                    title: qsTr("Display")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/display-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsDisplayPage.qml", {appWindow: ApplicationWindow.window})
                }

                AppTopicCard {
                    title: qsTr("Notifications")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/notification-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsNotificationPage.qml")
                }

                AppTopicCard {
                    title: qsTr("Sensors")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/sensor-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsSensorsPage.qml")
                }

                AppTopicCard {
                    title: qsTr("Weather")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "climate/weather-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsWeatherPage.qml")
                }

                AppTopicCard {
                    title: qsTr("Plant")
                    cardWidth: homeGrid.preferredCardWidth
                    iconSource: Style.assetsPath + "settings/watering-icon-white.svg"
                    iconSize: homeGrid.preferredIconSize
                    fontSize: Style.appTypography.fontSizeSubtitle
                    onActivated: root.stackView.push("SettingsPlantPage.qml")
                }
            }
        }
    }
}
