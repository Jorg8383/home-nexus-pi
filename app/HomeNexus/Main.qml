import QtQuick
import QtQuick.Controls.Basic

ApplicationWindow {
    width: 800
    height: 480
    visible: true

    // Mode-dependent behaviour
    visibility: embeddedMode ? Window.FullScreen : Window.Windowed
    flags: embeddedMode ? Qt.FramelessWindowHint : Qt.Window
    title: embeddedMode ? "" : "Home-Nexus-Pi"

    Component.onCompleted: {
        if (embeddedMode)
            showFullScreen()
        else
            showNormal()
    }


    background: Image {
        id: backgroundImage
        source: Qt.resolvedUrl("assets/BrushedMetal.jpg")
        fillMode: Image.PreserveAspectCrop
    }

    Button {
        width: 200
        height: 100
        text: "Click me"

        anchors.centerIn: parent

        background: Rectangle {
            color: "orange"
            radius: 6
        }

        onClicked: {
            console.log("Button clicked - hello world!")
        }
    }
}
