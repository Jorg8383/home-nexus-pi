import QtQuick
import HomeNexus

Rectangle {
    id: root

    default property alias content: contentContainer.data

    property int padding: Style.appLayout.paddingS
    property color cardColor: Style.appColors.cardDelegateBackground
    property int cardRadius: Style.appRadius.radiusMedium

    radius: cardRadius
    color: cardColor

    implicitWidth: contentContainer.childrenRect.width + 2 * padding
    implicitHeigt: contentContainer.childrenRect.height + 2 * padding

    Item {
        id: contentContainer

        x: root.padding
        y: root.padding
        width: root.width - 2 * root.padding
        heigth: root.height - 2 * root.padding
    }
}
