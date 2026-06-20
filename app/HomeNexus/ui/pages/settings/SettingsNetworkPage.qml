import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import HomeNexus

Page {
    id: root
    title: qsTr("Network Settings")
    background: null

    Image {
        source: Style.assetsPath + "under-construction.svg"
        height: 200
        width: 200
        fillMode: Image.PreserveAspectFit

        anchors.centerIn: parent
    }
}
