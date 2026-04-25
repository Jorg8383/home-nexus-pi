import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts


ToolBar {
    id: root

    property string title: ""
    property bool backButtonVisible: false

    signal backRequested()

    background: Rectangle {
        color: "#26282a"
    }

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 12

        ToolButton {
            text: "\u2190" // Unicode left arrow
            visible: root.backButtonVisible
            onClicked: root.backRequested()
        }

        Label {
            text: root.title
            font.pixelSize: 20
            elide: Text.ElideRight // handles text overflow --> "this is a long title ..."
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            font.weight: Font.DemiBold
        }
    }


}
