import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

AppCard {
    id: root

    property string title: "title"
    property url iconSource: ""
    property int iconSize: 100
    property int cardWidth: 220

    signal activated

    padding: Style.appLayout.paddingL

    implicitWidth: root.cardWidth

    contentItem: ColumnLayout {
        id: contentLayout
        spacing: Style.appLayout.spacingM

        Image {
            source: root.iconSource
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            Layout.preferredHeight: root.iconSize
            Layout.preferredWidth: root.iconSize

            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: root.title
            color: Style.appColors.textBright
            font.pixelSize: Style.appTypography.fontSizeTitle
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
        }
    }

    TapHandler {
        id: tapHandler
        gesturePolicy: TapHandler.DragThreshold
        onTapped: root.activated()
    }

    transformOrigin: Item.Center
    scale: tapHandler.pressed ? 0.9 : 1.0

    Behavior on scale {
        NumberAnimation {
            duration: 80
        }
    }
}
