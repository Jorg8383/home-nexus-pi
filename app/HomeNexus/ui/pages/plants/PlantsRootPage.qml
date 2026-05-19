import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: "Plants"
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
    //     text: "Open Plant Details"
    //     onClicked: root.stackView.push("PlantDetailPage01.qml")
    // }
}
