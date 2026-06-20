import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: qsTr("Display Settings")
    background: null

    required property var appWindow

    readonly property var backgroundOptions: [
        { text: qsTr("Brushed metal"), value: "brushedMetal" },
        { text: qsTr("Dark grey"), value: "darkGrey" }
    ]

    function backgroundIndex(mode) {
        for (let i = 0; i < backgroundOptions.length; ++i) {
            if (backgroundOptions[i].value === mode)
                return i
        }

        return 0
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.appLayout.marginsXL
        spacing: Style.appLayout.spacingM

        AppComboBoxCard {
            Layout.fillWidth: true

            labelText: qsTr("Background")

            model: root.backgroundOptions
            textRole: "text"
            valueRole: "value"

            currentIndex: root.backgroundIndex(root.appWindow.backgroundMode)

            onActivated: function(index, value) {
                if (!ApplicationWindow.window)
                    return

                root.appWindow.backgroundMode = value
                console.log("Background changed to:", value)
            }
        }

        // Invisble spacer item to push the visible elements to the top
        Item {
            Layout.fillHeight: true
        }
    }
}
