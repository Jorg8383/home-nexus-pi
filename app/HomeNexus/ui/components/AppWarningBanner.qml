import QtQuick
import QtQuick.Controls.Basic
import HomeNexus

Rectangle {
    id: root

    property string message: ""
    property bool hasWarning: false

    visible: root.hasWarning && root.message.length > 0
    implicitHeight: visible ? 25 : 0

    color: Style.appColors.warning

    clip: true

    Row {
        id:  contentRow

        anchors.fill: parent
        anchors.leftMargin: Style.appLayout.marginsL
        anchors.rightMargin: Style.appLayout.marginsL
        spacing: Style.appLayout.spacingL

        Image {
            id: warningSign
            source: Style.assetsPath + "warning-sign.svg"
            height: contentRow.height - 10
            fillMode: Image.PreserveAspectFit

            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: messageText

            width: parent.width - warningSign.width - contentRow.spacing
            height: contentRow.height

            text: root.message
            color: Style.appColors.textDark
            font.pixelSize: Style.appTypography.fontSizeBody

            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: bottomBorder

        height: 1
        color: Style.appColors.surfaceBright

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

}
