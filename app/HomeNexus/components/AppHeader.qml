import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus


ToolBar {
    id: root

    property string title: ""
    property bool backButtonVisible: false

    signal backRequested()

    background: Rectangle {
        color: Style.appColors.headerBackground
        height: Style.appSizes.headerHeight
    }

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left section
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: Style.appLayout.spacingM
                layoutDirection: Qt.LeftToRight

                ToolButton {
                    text: "\u2190" // Unicode left arrow
                    visible: root.backButtonVisible
                    onClicked: root.backRequested()
                }
            }

        }

        // Centre section
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                anchors.centerIn: parent
                text: root.title
                color: Style.appColors.headerText
                font.pixelSize: Style.appTypography.fontSizeTitle
                font.bold: true
                font.weight: Font.DemiBold
                elide: Text.ElideRight
            }
        }

        // Rigth section
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                anchors.rightMargin: Style.appLayout.spacingM
                layoutDirection: Qt.RightToLeft

                // Placeholder for future WiFi icons, etc
            }
        }

    }
}
