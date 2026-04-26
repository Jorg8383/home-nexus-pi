import QtQuick
import QtQuick.Controls.Basic
import HomeNexus

TabButton {
    id: root

    implicitHeight: Style.appSizes.tabButtonHeigth

    background: Rectangle {
        color: root.checked ? Style.appColors.tabButtonActiveBackground : Style.appColors.tabButtonInactiveBackground
    }

    contentItem: Text {
        text: root.text
        color: root.checked ? Style.appColors.tabButtonActiveText : Style.appColors.tabButtonInactiveText
        font.pixelSize: Style.appTypography.fontSizeTitle
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
