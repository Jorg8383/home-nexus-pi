import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../components"

Page {
    id: root
    title: "Home"
    background: null

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        Label {
            text: "Home Dashboard"
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Flat overview page"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Later: weather, climate summary, plant alerts, irrigation status"
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            Layout.preferredWidth: 320
            Layout.alignment: Qt.AlignHCenter
        }

        AppButton {
            text: "Cancel"
            onClicked: console.log("Cancel button clicked!")
        }
    }
}
