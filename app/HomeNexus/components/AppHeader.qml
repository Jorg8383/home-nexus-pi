import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus


ToolBar {
    id: root

    property string title: ""
    property bool backButtonVisible: false

    signal backRequested()

    height: Style.appSizes.headerHeight

    // Customized HeaderToolButton
    component HeaderToolButton : ToolButton {
        id: buttonRoot

        property color baseColor: Style.appColors.buttonBackground
        property color borderLight: Qt.lighter(baseColor, 1.2)
        property color borderDark: Qt.darker(baseColor, 1.4)

        implicitWidth: 100
        implicitHeight: 36

        background: Rectangle {
            radius: Style.appRadius.radiusSmall

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: buttonRoot.pressed ? buttonRoot.borderDark : buttonRoot.borderLight
                }
                GradientStop {
                    position: 1.0
                    color: buttonRoot.pressed ? buttonRoot.borderLight : buttonRoot.borderDark
                }
            }

            Rectangle {
                anchors.fill: parent
                anchors.margins: 3
                radius: parent.radius - 2
                color: buttonRoot.baseColor
            }
        }

        contentItem: Item {
            Text {
                anchors.centerIn: parent
                text: buttonRoot.text
                color: Style.appColors.buttonText
                font.pixelSize: 48
            }
        }

    }

    // Header background
    background: Rectangle {
        color: Style.appColors.headerBackground
        height: Style.appSizes.headerHeight

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: Style.appColors.surfaceBright
        }
    }

    // Header content
    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left content section
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: Style.appLayout.spacingM
                layoutDirection: Qt.LeftToRight

                HeaderToolButton {
                    text: "\u21A9"// Unicode return arrow
                    visible: root.backButtonVisible
                    onClicked: root.backRequested()
                }
            }

        }

        // Centre content section
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

        // Right content section
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
