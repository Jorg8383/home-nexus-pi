import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Settings"
    background: null

    required property StackView stackView

    Image {
        source: Style.assetsPath + "under-construction.svg"
        height: 200
        width: 200
        fillMode: Image.PreserveAspectFit

        anchors.centerIn: parent
    }

    // Button {
    //     text: "Open Settings Details"
    //     onClicked: root.stackView.push("NetworkSettingsPage.qml")
    // }
}
