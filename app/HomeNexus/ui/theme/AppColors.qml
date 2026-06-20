import QtQuick

QtObject {
    // Base palette
    readonly property color cardBackground: "#80000000" // black with 50% opacity
    readonly property color cardDelegateBackground: "#FF000000" // black with 100% opacity

    readonly property color surfaceDark: "#FF202020"
    readonly property color surfaceBright: "#FFE6E6E6"

    readonly property color textDark: "#FF202020"
    readonly property color textBright: "#FFE6E6E6"

    readonly property color contentBackground: "#FF454545"

    // Header / footer
    readonly property color headerBackground: surfaceDark
    readonly property color footerBackground: surfaceBright

    readonly property color tabButtonActiveBackground: surfaceBright
    readonly property color tabButtonInactiveBackground: surfaceDark

    readonly property color tabButtonActiveText: textDark
    readonly property color tabButtonInactiveText: textBright

    readonly property color headerText: textBright

    // Controls
    readonly property color buttonBackground: surfaceBright
    readonly property color buttonText: textDark
    readonly property color warning: "#FFA300"
}
