import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Settings"
    background: null

    required property StackView stackView

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Settings Overview"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "Open Settings Details"
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.stackView.push("NetworkSettingsPage.qml")
        }
    }
}
