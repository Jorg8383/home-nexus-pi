import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

AppCard {
    id: root

    property string title: ""
    property string value: ""
    property string unit: ""
    property url iconSource: ""

    padding: Style.appLayout.paddingL

    implicitWidth: 220
    implicitHeight: contentLayout.implicitHeight + 2 * padding

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        spacing: Style.appLayout.spacingM

        Label {
            text: root.title
            font.pixelSize: Style.appTypography.fontSizeTitle
            color: Style.appColors.textBright
            elide: Text.ElideRight

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
        }

        RowLayout {
            spacing: Style.appLayout.spacingXS
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: Style.appLayout.marginsL

            Image {
                source: root.iconSource
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 58
                Layout.preferredWidth: 58
            }

            Label {
                text: root.value + " " + root.unit
                font.pixelSize: Style.appTypography.fontSizeHeadline
                color: Style.appColors.textBright

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
