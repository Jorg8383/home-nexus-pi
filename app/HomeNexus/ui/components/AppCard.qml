import QtQuick
import QtQuick.Controls.Basic
import HomeNexus

Pane {
    id: root

    property color cardColor: Style.appColors.cardBackground
    property int cardRadius: Style.appRadius.radiusMedium

    padding: Style.appLayout.paddingM

    background: Rectangle {
        radius: root.cardRadius
        color: root.cardColor
    }
}
