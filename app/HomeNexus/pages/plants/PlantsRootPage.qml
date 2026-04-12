import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "plants"
    background: null

    required property StackView stackView

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Plants Overview"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "Open Plant Details"
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.stackView.push("PlantDetailPage01.qml")
        }
    }
}
