// AppInputTextCard.qml
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

AppCard {
    id: root

    property alias labelText: titleLabel.text
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText

    property int textFieldPreferredWidth: 220
    property int textFieldFontPixelSize: Style.appTypography.fontSizeSmall
    property int labelFontPixelSize: Style.appTypography.fontSizeSubtitle

    property int debounceIntervalMs: 800
    property int minimumInputLength: 3
    property bool trimInput: true
    property bool submitOnTextEdited: true

    signal submitted(string text)

    implicitHeight: contentLayout.implicitHeight + padding * 2

    padding: Style.appLayout.paddingL

    function normalizedText() {
        return trimInput ? textField.text.trim() : textField.text
    }

    function submit() {
        const value = normalizedText()

        if (value.length < minimumInputLength)
            return

        debounceTimer.stop()
        submitted(value)
    }

    RowLayout {
        id: contentLayout

        anchors.fill: parent
        spacing: Style.appLayout.spacingM

        Label {
            id: titleLabel

            Layout.fillWidth: true

            color: Style.appColors.textBright

            font.pixelSize: root.labelFontPixelSize
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        TextField {
            id: textField

            Layout.preferredWidth: root.textFieldPreferredWidth
            Layout.fillWidth: false

            font.pixelSize: root.textFieldFontPixelSize
            selectByMouse: true

            color: Style.appColors.textDark
            inputMethodHints: Qt.ImhNoPredictiveText

            onTextEdited: {
                if (root.submitOnTextEdited)
                    debounceTimer.restart()
            }

            onAccepted: {
                root.submit()
            }
        }
    }

    Timer {
        id: debounceTimer

        interval: root.debounceIntervalMs
        repeat: false

        onTriggered: {
            root.submit()
        }
    }
}
