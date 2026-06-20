// AppCheckBoxCard.qml
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

AppCard {
    id: root

    property alias labelText: titleLabel.text
    property alias checked: checkBox.checked
    property alias checkable: checkBox.checkable

    property int checkBoxPreferredWidth: 48
    property int labelFontPixelSize: Style.appTypography.fontSizeSubtitle

    signal toogled(bool checked)

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

        CheckBox {
            id: checkBox

            Layout.preferredWidth: root.checkBoxPreferredWidth
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            onToggled: {
                root.toogled(checked)
            }
        }
    }
}
