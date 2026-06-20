// AppComboBoxCard.qml
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

AppCard {
    id: root

    property alias labelText: titleLabel.text
    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property alias textRole: comboBox.textRole
    property alias valueRole: comboBox.valueRole

    readonly property alias currentText: comboBox.currentText
    readonly property alias currentValue: comboBox.currentValue

    property int comboBoxPreferredWidth: 180
    property int comboBoxFontPixelSize: Style.appTypography.fontSizeSmall
    property int labelFontPixelSize: Style.appTypography.fontSizeSubtitle

    signal activated(int index, var value)

    implicitHeight: contentLayout.implicitHeight + padding * 2

    padding: Style.appLayout.paddingL

    RowLayout {
        id: contentLayout
        anchors.fill: parent
        spacing: Style.appLayout.spacingM

        Label {
            id: titleLabel

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            color: Style.appColors.textBright
            font.pixelSize: root.labelFontPixelSize

            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        ComboBox {
            id: comboBox

            Layout.preferredWidth: root.comboBoxPreferredWidth
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            font.pixelSize: root.comboBoxFontPixelSize

            onActivated: function(index) {
                root.activated(index, currentValue)
            }
        }
    }
}
