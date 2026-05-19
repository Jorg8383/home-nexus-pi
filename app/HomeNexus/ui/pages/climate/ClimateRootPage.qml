import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: "Climate"
    background: null

    required property StackView stackView

    RowLayout {
        anchors.centerIn: parent
        spacing: Style.appLayout.spacingXL

        AppTopicCard {
            title: qsTr("Greenhouse")
            iconSource: Style.assetsPath + "/climate/greenhouse-white.svg"
            onActivated: root.stackView.push("ClimateGreenhousePage.qml")
        }

        AppTopicCard {
            title: qsTr("Indoor")
            iconSource: Style.assetsPath + "/climate/room-white.svg"
            onActivated: root.stackView.push("ClimateIndoorPage.qml")
        }

        AppTopicCard {
            title: qsTr("Weather")
            iconSource: Style.assetsPath + "/climate/weather-white.svg"
            onActivated: root.stackView.push("ClimateWeatherPage.qml")
        }
    }
}
