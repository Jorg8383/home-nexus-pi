import QtQuick
import QtQuick.Controls.Basic
import HomeNexus

TabButton {
    id: root

    property string iconName: ""

    implicitHeight: Style.appSizes.tabButtonHeigth

    background: Rectangle {
        color: root.checked ? Style.appColors.tabButtonActiveBackground : Style.appColors.tabButtonInactiveBackground
    }

    contentItem: Item {
        implicitWidth: 50
        implicitHeight: 50

        Image {
            anchors.centerIn:  parent
            width: 36
            height: 36
            source: `${Style.assetsPath}tabbar/${root.iconName}-${root.checked ? "dark" : "white"}.svg`
            fillMode: Image.PreserveAspectFit
        }
    }
}
