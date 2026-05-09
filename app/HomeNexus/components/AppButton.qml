import QtQuick
import QtQuick.Controls.Basic
import HomeNexus

Button {
    id: root

    property color baseColor: Style.appColors.buttonBackground
    property color borderLight: Qt.lighter(baseColor, 1.2)
    property color borderDark: Qt.darker(baseColor, 1.4)

    implicitWidth: 120
    implicitHeight: 48

    background: Rectangle {
        radius: Style.appRadius.radiusSmall

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: root.pressed ? root.borderDark : root.borderLight
            }
            GradientStop {
                position: 1.0
                color: root.pressed ? root.borderLight : root.borderDark
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            radius: parent.radius - 2
            color: root.baseColor
        }
    }

    contentItem: Text {
        text: root.text
        color: Style.appColors.buttonText
        font.pixelSize: Style.appTypography.fontSizeButton
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
