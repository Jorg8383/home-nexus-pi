import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Climate"
    background: null

    required property StackView stackView

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Climate Overview"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "Indoor Climate"
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.stackView.push("ClimateIndoorsPage.qml")
        }

        Button {
            text: "Weather Forecast"
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.stackView.push("ClimateWeatherPage.qml")
        }
    }
}
